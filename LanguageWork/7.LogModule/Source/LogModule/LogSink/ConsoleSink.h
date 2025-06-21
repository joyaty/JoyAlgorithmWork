
#pragma once

#include "ILogSink.h"
#include "LogEntry.h"
#include <mutex>

namespace Joy
{
    /**
     * @brief 控制台日志输出端
     *
     */
    class ConsoleSink : public ILogSink
    {
    public:
        /**
         * @brief 写入控制台日志
         *
         * @param logEntry
         */
        virtual void Write(const LogEntry& logEntry) override;

        /**
         * @brief 刷新控制台日志缓冲
         *
         */
        virtual void Flush() override;

    private:
        /**
         * @brief 格式化LogEntry日志信息为输出字符串
         * 
         * @param logEntry 
         * @return std::string 
         */
        std::string FormateEntry(const LogEntry& logEntry);

    private:
        std::mutex m_Mutex{};
    };
}   // namespace Joy