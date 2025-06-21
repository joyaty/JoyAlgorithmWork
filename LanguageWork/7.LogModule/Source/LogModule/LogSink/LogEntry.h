
#pragma once

#include <cstdint>
#include <ctime>
#include <string>
#include <thread>

namespace Joy
{
    /**
     * @brief 日志级别枚举
     *
     */
    enum class EnumLogLevel : uint8_t
    {
        TRACE,   // 细粒度信息
        DEBUG,   // 调试
        INFO,    // 信息
        WARN,    // 警告
        ERROR,   // 错误
        FATAL,   // 崩溃
    };

    /**
     * @brief 描述日志信息的数据结构
     *
     */
    struct LogEntry
    {
        EnumLogLevel    logLevel{};     // 日志等级
        std::string     message{};      // 日志内容
        std::time_t     timestamp{};    // 日志时间
        std::thread::id thread_id{};    // 打印线程
        std::string     sourceFile{};   // 关联源文件
        int             sourceLine;     // 源文件行号

        /**
         * @brief 日志等级枚举转描述字符串
         *
         * @param level
         * @return std::string
         */
        static std::string LevelToString(EnumLogLevel level)
        {
            switch (level)
            {
                case EnumLogLevel::TRACE: return "TRACE";
                case EnumLogLevel::DEBUG: return "DEBUG";
                case EnumLogLevel::INFO: return "INFO";
                case EnumLogLevel::WARN: return "WARN";
                case EnumLogLevel::ERROR: return "ERROR";
                case EnumLogLevel::FATAL: return "FATAL";
                default: return "UNKNOWN";
            }
        }
    };
}   // namespace Joy