
#pragma once

#include <cstddef>
#include <cstdint>
#include <mutex>
#include <queue>
#include <string>

namespace Joy
{
    class CircleBuffer;

    /**
     * @brief 消息包解析器
     *
     */
    class PacketParser final
    {
    public:
        /**
         * @brief 消息包数据结构定义
         *
         */
        struct Packet
        {
            /**
             * @brief 消息头的固定长度
             *
             */
            static const size_t kHeaderSize = 6;

            /**
             * @brief 校验位1
             *
             */
            static const uint8_t kCheckCode1 = 0x5A;

            /**
             * @brief 校验位2
             *
             */
            static const uint8_t kCheckCode2 = 0xA5;

            /**
             * @brief 消息内容
             *
             */
            std::string msgBody;

            Packet(const std::string& msg)
                : msgBody(msg)
            {}
        };

    private:
        enum class EnumParseStep : uint8_t
        {
            kStepParseHeader,
            kStepParseBody,
        };

    public:
        PacketParser();
        PacketParser(const PacketParser&) = delete;
        PacketParser(PacketParser&&)      = delete;

    public:
        PacketParser& operator=(const PacketParser&) = delete;
        PacketParser& operator=(PacketParser&&)      = delete;

    public:
        /**
         * @brief 读取接收缓冲区，解析为消息包
         *
         * @param recvBuffer
         */
        void Parse(CircleBuffer& recvBuffer);

        /**
         * @brief 交换前后缓冲，由逻辑线程读取全部前缓冲消息后，交换
         *
         */
        void Swap();

        /**
         * @brief 获取前缓冲，逻辑线程
         *
         * @return std::queue<Packet>&
         */
        std::queue<Packet>& GetFront() const { return *m_pFront; }

        /**
         * @brief 获取后缓冲，接受线程
         *
         * @return std::queue<Packet>&
         */
        std::queue<Packet>& GetBack() const { return *m_pBack; }

    private:
        // 当前的解析步骤
        EnumParseStep m_eParseStep;
        // 当前解析的包体长度
        size_t m_nCurrentPackBodySize;

        // 前缓冲 - 总是由逻辑线程读取
        std::queue<Packet>* m_pFront;
        // 后缓冲 - 总是由接收线程写入
        std::queue<Packet>* m_pBack;

    private:
        // 线程锁，用于多线程缓冲区操作安全
        std::mutex m_Mutex{};
    };
}   // namespace Joy