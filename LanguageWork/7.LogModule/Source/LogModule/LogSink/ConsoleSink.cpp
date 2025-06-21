
#include "ConsoleSink.h"
#include <ctime>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>

namespace Joy
{
    void ConsoleSink::Write(const LogEntry& logEntry)
    {
        std::string outputMsg(std::move(FormateEntry(logEntry)));
        std::lock_guard<std::mutex> lock_guard(m_Mutex);
        std::cout << outputMsg << std::endl;
    }

    void ConsoleSink::Flush()
    {
        std::lock_guard<std::mutex> lock_guard(m_Mutex);
        std::cout << std::flush;
    }

    std::string ConsoleSink::FormateEntry(const LogEntry& logEntry)
    {
        std::ostringstream sstream{};

        // 时间戳
        auto timestamp = *std::localtime(&logEntry.timestamp);
        sstream << std::put_time(&timestamp, "%Y-%m-%d %H:%M:%S") << " ";
        // 日志级别
        sstream << "[" << LogEntry::LevelToString(logEntry.logLevel) << "] ";
        // 日志线程
        sstream << "[TID:" << logEntry.thread_id << "] ";
        // 原位置
        if (!logEntry.sourceFile.empty())
        {
            auto fileName = std::filesystem::path(logEntry.sourceFile).filename().string();
            sstream << "[" << fileName << ":" << logEntry.sourceLine << "] ";
        }
        // 日志内容
        sstream << logEntry.message;

        return sstream.str();
    }
}   // namespace Joy
