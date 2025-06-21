
#include "AsyncLogger.h"
#include "LogSink/LogEntry.h"
#include <chrono>
#include <memory>
#include <mutex>
#include <vector>

namespace Joy
{
    AsyncLogger::AsyncLogger()
        : m_IsRunning(true)
        , m_LogThread(&AsyncLogger::ProcessLogEntries, this)
        , m_MinLevel(EnumLogLevel::DEBUG)
    {
    }

    AsyncLogger::~AsyncLogger()
    {
        // 关闭日志线程
        m_IsRunning = false;
        if (m_LogThread.joinable())
        {
            m_LogThread.join();
        }
        // 处理未被日志线程及时处理的剩余日志
        ProcessRemaining();
    }

    // /**
    //  * @brief 模板特化 - 添加ConsoleSink
    //  *
    //  * @tparam ConsoleSink
    //  * @return int
    //  */
    // template<>
    // int AsyncLogger::AddSink<ConsoleSink>()
    // {
    //     std::lock_guard<std::mutex> lock_guard(m_SinksOptMutex);
    //     m_Sinks.emplace_back(std::make_unique<ConsoleSink>());
    //     return static_cast<int>(m_Sinks.size()) - 1;
    // }

    void AsyncLogger::Log(LogEntry&& logEntry)
    {
        {
            std::lock_guard<std::mutex> lock_guard(m_QueueMutex);
            m_LogQueue.push(std::move(logEntry));
        }
        // 条件变量通知新增日志打印
        m_Condition.notify_one();
    }

    void AsyncLogger::Flush()
    {
        std::unique_lock<std::mutex> lock(m_QueueMutex);
        m_ConditionFlush.wait(lock, [this]()
                              { return m_LogQueue.empty(); });
        // 刷新各个日志输出端的日志缓冲
        std::lock_guard<std::mutex> lock_guard(m_SinksOptMutex);
        for (auto& sink : m_Sinks)
        {
            sink->Flush();
        }
    }

    void AsyncLogger::ProcessLogEntries()
    {
        // 批量日志打印操作缓冲
        std::vector<LogEntry> batchOpts;
        batchOpts.reserve(32);

        while (m_IsRunning)
        {
            std::unique_lock<std::mutex> lock(m_QueueMutex);
            // 条件变量+锁，等待日志队列不为空或者日志线程即将被关闭，等待超时100ms
            m_Condition.wait_for(lock, std::chrono::milliseconds(100), [this]()
                                 { return !m_LogQueue.empty() || !m_IsRunning; });

            if (!m_LogQueue.empty())
            {   // 获取队列中待处理的日志，写入批量缓冲中
                while (!m_LogQueue.empty() && batchOpts.size() < batchOpts.capacity())
                {
                    batchOpts.emplace_back(std::move(m_LogQueue.front()));
                    m_LogQueue.pop();
                }
            }
            // 写入日志
            if (!batchOpts.empty())
            {
                BatchWrite(batchOpts);
                batchOpts.clear();
                // 通知等待刷新的线程
                m_ConditionFlush.notify_all();
            }
        }
    }

    void AsyncLogger::BatchWrite(const std::vector<LogEntry>& batches)
    {
        std::lock_guard<std::mutex> lock_guard(m_SinksOptMutex);
        for (const auto& entry : batches)
        {
            if (entry.logLevel < m_MinLevel)
            {
                continue;
            }
            // 写入所有的日志输出端
            for (auto& sink : m_Sinks)
            {
                sink->Write(entry);
            }
        }
    }

    void AsyncLogger::ProcessRemaining()
    {
        std::vector<LogEntry> remaining;
        {   // 获取日志队列中全部的日志，写入批量缓冲
            std::lock_guard<std::mutex> lock(m_QueueMutex);
            while (!m_LogQueue.empty())
            {
                remaining.emplace_back(std::move(m_LogQueue.front()));
                m_LogQueue.pop();
            }
        }

        if (!remaining.empty())
        {   // 写入日志
            BatchWrite(remaining);
            // 立刻刷新日志缓冲
            std::lock_guard<std::mutex> sink_lock(m_SinksOptMutex);
            for (auto& sink : m_Sinks)
            {
                sink->Flush();
            }
        }
    }

}   // namespace Joy