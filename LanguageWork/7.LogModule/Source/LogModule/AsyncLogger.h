
#pragma once

#include "LogSink/ILogSink.h"
#include "LogSink/LogEntry.h"
#include <atomic>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace Joy
{
    /**
     * @brief 异步日志器
     *
     */
    class AsyncLogger final
    {
    public:
        AsyncLogger();
        ~AsyncLogger();

    public:
        /**
         * @brief 设置日志等级
         *
         * @param level
         */
        inline void SetLevel(EnumLogLevel level) { m_MinLevel = level; }

        /**
         * @brief 附加日志输出端
         *
         * @tparam T ILogSink的实现端类型
         * @return int 日志输出端的索引，用于指定输出到目标端
         */
        template<typename T>
        int AddSink()
        {
            std::lock_guard<std::mutex> lock_guard(m_SinksOptMutex);
            m_Sinks.emplace_back(std::make_unique<T>());
            return static_cast<int>(m_Sinks.size()) - 1;
        }

        /**
         * @brief 添加日志信息
         *
         * @param logEntry
         */
        void Log(LogEntry&& logEntry);

        /**
         * @brief 刷新日志缓冲
         *
         */
        void Flush();

    private:
        /**
         * @brief 处理日志打印，日志线程执行入口
         *
         */
        void ProcessLogEntries();

        /**
         * @brief 批量写入缓冲中的日志
         *
         * @param batches 日志缓冲
         */
        void BatchWrite(const std::vector<LogEntry>& batches);

        /**
         * @brief 处理剩余的日志信息，在日志模块被关闭前调用
         *
         */
        void ProcessRemaining();

    private:
        // 日志队列
        std::queue<LogEntry> m_LogQueue{};
        // 日志操作锁
        std::mutex m_QueueMutex{};
        // 日志同步条件变量
        std::condition_variable m_Condition{};
        // 日志刷新条件变量
        std::condition_variable m_ConditionFlush{};

        // 附加的日志输出端集合
        std::vector<std::unique_ptr<ILogSink>> m_Sinks{};
        // 输出端集合操作锁
        std::mutex m_SinksOptMutex{};

        // 日志级别过滤
        std::atomic<EnumLogLevel> m_MinLevel;
        // 日志线程工作状态
        std::atomic<bool> m_IsRunning;
        // 日志线程
        std::thread m_LogThread;
    };
}   // namespace Joy