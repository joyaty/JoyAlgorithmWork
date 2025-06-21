
#pragma once

#include "AsyncLogger.h"
#include "LogSink/LogEntry.h"
#include <memory>

namespace Joy
{
    class LogUtil final
    {
    public:
        /**
         * @brief 单例模式 - 获取日志模块单例
         *
         * @return LogUtil&
         */
        static LogUtil& Instance();

    public:
        /**
         * @brief 初始化日志模块，系统启动调用
         *
         * @param filePath 日志文件路径
         * @param level 日志过滤等级
         */
        void Init(const std::string& filePath = "", EnumLogLevel level = EnumLogLevel::INFO);

        /**
         * @brief 设置日志过滤等级
         *
         * @param level
         */
        void SetLevel(EnumLogLevel level);

        /**
         * @brief 添加日志输出
         *
         * @param level 日志等级
         * @param content 日志内容
         * @param file 日志关联源文件
         * @param line 日志关联行号
         */
        void Log(EnumLogLevel level, const std::string& content, const char* file = "", int line = 0);

        /**
         * @brief 刷新日志缓冲
         *
         */
        void Flush();

    private:
        /**
         * @brief 单例模式私有构造
         *
         */
        LogUtil() = default;
        // 单例模式 - 删除拷贝复制构造和拷贝赋值操作
        LogUtil(const LogUtil&)            = delete;
        LogUtil& operator=(const LogUtil&) = delete;

    private:
        // 异步日志器
        std::unique_ptr<AsyncLogger> m_AsyncLogger;
    };
}   // namespace Joy

// 日志宏声明
#define LOG_TRACE(content) Joy::LogUtil::Instance().Log(Joy::EnumLogLevel::TRACE, content, __FILE__, __LINE__)
#define LOG_DEBUG(content) Joy::LogUtil::Instance().Log(Joy::EnumLogLevel::DEBUG, content, __FILE__, __LINE__)
#define LOG_INFO(content) Joy::LogUtil::Instance().Log(Joy::EnumLogLevel::INFO, content, __FILE__, __LINE__)
#define LOG_WARN(content) Joy::LogUtil::Instance().Log(Joy::EnumLogLevel::WARN, content, __FILE__, __LINE__)
#define LOG_ERROR(content) Joy::LogUtil::Instance().Log(Joy::EnumLogLevel::ERROR, content, __FILE__, __LINE__)
#define LOG_FATAL(content) Joy::LogUtil::Instance().Log(Joy::EnumLogLevel::FATAL, content, __FILE__, __LINE__)