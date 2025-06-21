
#pragma once

#include "LogEntry.h"

namespace Joy
{
    /**
     * @brief 日志输出端抽象接口
     * 
     */
    class ILogSink
    {
    public:
        /**
         * @brief 虚析构
         * 
         */
        virtual ~ILogSink() = default;

    public:
        /**
         * @brief 写入日志
         * 
         * @param logEntry 
         */
        virtual void Write(const LogEntry& logEntry) = 0;

        /**
         * @brief 刷新日志缓冲
         * 
         */
        virtual void Flush() = 0;
    };
}   // namespace Joy