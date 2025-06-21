
#include "LogUtil.h"
#include "LogSink/ConsoleSink.h"
#include "LogSink/LogEntry.h"
#include <thread>

namespace Joy
{
    LogUtil& LogUtil::Instance()
    {
        static LogUtil logger;
        return logger;
    }

    void LogUtil::Init(const std::string& filePath /* = "" */, EnumLogLevel level /* = EnumLogLevel::INFO */)
    {
        m_AsyncLogger = std::make_unique<AsyncLogger>();
        m_AsyncLogger->SetLevel(level);
        // 默认添加控制台日志输出
        m_AsyncLogger->AddSink<ConsoleSink>();
        // TODO 文件日志系统
    }

    void LogUtil::SetLevel(EnumLogLevel level)
    {
        if (m_AsyncLogger)
        {
            m_AsyncLogger->SetLevel(level);
        }
    }

    void LogUtil::Log(EnumLogLevel level, const std::string& content, const char* file /* = "" */, int line /* = 0 */)
    {
        if (m_AsyncLogger)
        {
            LogEntry entry{
                level,
                content,
                std::time(nullptr),
                std::this_thread::get_id(),
                file ? file : "",
                line};

            m_AsyncLogger->Log(std::move(entry));
        }
    }

    void LogUtil::Flush()
    {
        if (m_AsyncLogger)
        {
            m_AsyncLogger->Flush();
        }
    }

}   // namespace Joy