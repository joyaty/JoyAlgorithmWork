
#pragma once

namespace Joy
{
    // 唯一所有权智能指针，自动释放所指向的内存区域
    template<typename T> class JoyUniquePointer final
    {
    public:
        // 默认构造函数
        JoyUniquePointer()
            : m_RawPointer(nullptr)
        {}
        // 从裸指针构造唯一所有权指针
        JoyUniquePointer(T* pSrcPointer)
            : m_RawPointer(pSrcPointer)
        {}
        // 禁止拷贝构造函数
        JoyUniquePointer(const JoyUniquePointer& pSrcPointer) = delete;
        // 移动构造函数
        JoyUniquePointer(JoyUniquePointer&& pSrcPointer) noexcept
        {
            if (this == &pSrcPointer)
            {
                return;
            }
            // 原始指针有执行内存块，释放掉
            Release();
            // 转移原始指针的所有权
            m_RawPointer             = pSrcPointer.m_RawPointer;
            pSrcPointer.m_RawPointer = nullptr;
        }
        // 析构函数，释放指针指向的内存区域
        ~JoyUniquePointer() { Release(); }

    public:
        // 获取原始裸指针
        T* Get() const { return m_RawPointer; }

    private:
        void Release()
        {
            if (!m_RawPointer)
            {
                return;
            }
            delete m_RawPointer;
            m_RawPointer = nullptr;
        }

    public:
        // 禁止拷贝赋值操作
        JoyUniquePointer& operator=(const JoyUniquePointer& pSrcPointer) = delete;
        // 移动赋值操作
        JoyUniquePointer& operator=(JoyUniquePointer&& pSrcPointer) noexcept
        {
            if (this == &pSrcPointer)
            {
                return *this;
            }
            // 原始指针有执行内存块，释放掉
            Release();
            // 转移原始指针的所有权
            m_RawPointer             = pSrcPointer.m_RawPointer;
            pSrcPointer.m_RawPointer = nullptr;
            return *this;
        }
        // 重写解引用操作，实现与裸指针相同的解引用操作
        T& operator*() const { return *m_RawPointer; }
        // 重写指针操作，允许通过指针调用成员或方法
        T* operator->() const { return m_RawPointer; }

    private:
        // 原始裸指针
        T* m_RawPointer{nullptr};
    };
}   // namespace Joy