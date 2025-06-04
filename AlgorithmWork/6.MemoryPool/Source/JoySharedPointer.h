
#pragma once

namespace Joy
{
    // 引用计数控制块
    struct ControlBlock
    {
        int refCount;
    };

    // 共享所有权指针，可以有多个所有者
    template<typename T> class JoySharedPointer final
    {
    public:
        JoySharedPointer()
            : m_pRawResource(nullptr)
            , m_pControlBlock(nullptr)
        {}

        explicit JoySharedPointer(T* pNakeResource)
            : m_pRawResource(pNakeResource)
        {
            if (m_pRawResource == nullptr)
            {   // 内存指针为空，不构建引用计数控制块
                m_pControlBlock = nullptr;
            }
            else
            {   // 构建引用计数控制块
                m_pControlBlock = new ControlBlock();
                Retain();
            }
        }

        JoySharedPointer(const JoySharedPointer& pShared)
            : m_pRawResource(pShared.m_pRawResource)
            , m_pControlBlock((pShared.m_pControlBlock))
        {
            Retain();
        }

        JoySharedPointer(JoySharedPointer&& pShared) noexcept
            : m_pRawResource(pShared.m_pRawResource)
            , m_pControlBlock(pShared.m_pControlBlock)
        {
            pShared.m_pControlBlock = nullptr;
            pShared.m_pRawResource  = nullptr;
        }

        ~JoySharedPointer()
        {
            if (m_pRawResource != nullptr)
            {
                Release();
            }
        }

    public:
        // 获取原始裸指针
        T* Get() const { return m_pRawResource; }

        // 获取资源引用计数
        int GetRefCount() const
        {
            return m_pControlBlock == nullptr ? 0 : m_pControlBlock->refCount;
        }

    public:
        // 重载拷贝赋值运算符
        JoySharedPointer& operator=(const JoySharedPointer& pShared)
        {
            if (this == pShared)
            {   // 相同指针，不操作
                return *this;
            }

            if (m_pRawResource != nullptr)
            {   // 原有资源不为空，释放占用
                Release();
            }
            // 拷贝所有权
            m_pRawResource  = pShared.m_pRawResource;
            m_pControlBlock = pShared.m_pControlBlock;
            // 引用计数+1
            Retain();
            return *this;
        }
        // 重载移动赋值运算符
        JoySharedPointer& operator=(JoySharedPointer&& pShared) noexcept
        {
            if (this == pShared)
            {   // 相同指针，不操作
                return *this;
            }
            if (m_pRawResource != nullptr)
            {   // 原有资源不为空，释放占用
                Release();
            }
            // 转移所有权
            m_pRawResource  = pShared.m_pRawResource;
            m_pControlBlock = pShared.m_pControlBlock;
            // 释放被移动指针的所有权
            pShared.m_pRawResource  = nullptr;
            pShared.m_pControlBlock = nullptr;
            return *this;
        }

        // 重写解引用操作，实现与裸指针相同的效果
        JoySharedPointer& operator*() const { return *m_pRawResource; }
        // 重写指针操作，实现与裸指针相同的效果
        JoySharedPointer* operator->() const { return m_pRawResource; }

    private:
        // 引用计数 + 1
        void Retain() const { ++(m_pControlBlock->refCount); }
        // 引用计数 - 1
        void Release()
        {
            --(m_pControlBlock->refCount);
            if (m_pControlBlock->refCount == 0)
            {   // 引用计数为0，释放内存
                delete m_pRawResource;
                delete m_pControlBlock;
            }
            m_pRawResource  = nullptr;
            m_pControlBlock = nullptr;
        }

    private:
        // 指向的内存资源区域
        T* m_pRawResource{};
        // 额外的引用计数控制块
        ControlBlock* m_pControlBlock{};
    };
}   // namespace Joy
