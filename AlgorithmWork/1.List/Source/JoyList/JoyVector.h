/**
 * 自定义ADT - 向量
 * 实现STL中vector
 */

#pragma once

#include <cstdlib>

namespace Joy
{
    /// <summary>
    /// 自定义Vector
    /// </summary>
    /// <typeparam name="T"></typeparam>
    template<typename T>
    class JoyVector final
    {
    public:
#pragma region 自定义列表迭代器
        // 迭代器类
        class Iterator
        {
        public:
            using self_type  = typename JoyVector<T>::Iterator;
            using value_type = T;
            using reference  = T&;
            using pointer    = T*;

        public:
            Iterator(T* pData)
                : m_pData(pData)
            {}

        public:
            // 重载解引用操作符
            reference operator*() const { return *m_pData; }
            // 重载指针操作符
            pointer operator->() const { return m_pData; }

            // 重载前置++操作符
            self_type& operator++()
            {
                if (m_pData != nullptr)
                {
                    ++m_pData;
                }
                return *this;
            }

            // 重载后置++操作符
            self_type operator++(int)
            {
                self_type temp = *this;
                ++(*this);   // 调用前置++操作符
                return temp;
            }

            // 重载+操作符，迭代器向后偏移指定长度
            self_type operator+(int offset)
            {
                self_type temp = *this;
                temp.m_pData += offset;
                return temp;
            }

            // 重写前置--操作
            self_type& operator--()
            {
                if (m_pData != nullptr)
                {
                    --m_pData;
                }
                return *this;
            }

            // 重写后置--操作符
            self_type operator--(int)
            {
                self_type temp = *this;
                --(*this);
                return temp;
            }

            // 重载-操作符，迭代器向前偏移指定长度
            self_type operator-(int offset)
            {
                self_type temp = this;
                temp.m_pData -= offset;
                return temp;
            }

            // 重载相等判断操作符
            bool operator==(const self_type& rhs) const { return m_pData == rhs.m_pData; }

            // 重载重写不等判断操作符
            bool operator!=(const self_type& rhs) const { return !(*this == rhs); }

        private:
            T* m_pData;
        };
#pragma endregion

    public:
        using value_type     = T;
        using const_Iterator = typename JoyVector<value_type>::Iterator;
        using iterator       = typename JoyVector<value_type>::Iterator;

    public:
        // 默认构造函数
        JoyVector()
            : m_pData(nullptr)
            , m_nCapacity(0)
            , m_nSize(0)
        {}
        //
        explicit JoyVector(int capacity)
            : m_nSize(0)
        {
            Reserve(capacity);
        }

        // 拷贝构造函数
        JoyVector(const JoyVector& rhs)
            : m_nCapacity(rhs.Capacity())
            , m_nSize(rhs.Size())
        {
            operator=(rhs);
        }

        // 移动构造函数
        JoyVector(JoyVector&& rhs) noexcept
            : m_pData(rhs.m_pData)
            , m_nCapacity(rhs.Capacity())
            , m_nSize((rhs.Size()))
        {
            rhs.m_pData     = nullptr;
            rhs.m_nCapacity = 0;
            rhs.m_nSize     = 0;
        }

        ~JoyVector()
        {
            m_nCapacity = 0;
            m_nSize     = 0;
            std::free(m_pData);
            m_pData = nullptr;
        }

    public:
        // 获取向量容量
        int Capacity() const { return m_nCapacity; }
        // 获取向量数据长度
        int Size() const { return m_nSize; }
        // 向量是否为空
        bool Empty() const { return Size() == 0; }
        // 清空向量
        void Clear() { m_nSize = 0; }
        // 向量尾部添加元素
        void Push_Back(const value_type& value)
        {
            if (m_nSize == m_nCapacity)
            { // 容量已满，扩容
                Reserve(m_nCapacity * 2 + 1);
            }
            m_pData[m_nSize] = value;
            ++m_nSize;
        }
        // 删除尾部元素
        void Pop_Back() { m_nSize = m_nSize > 0 ? m_nSize - 1 : 0; }
        // 获取尾部元素
        const value_type& Back() const { return m_pData[m_nSize - 1]; }
        // 获取头部元素
        const value_type& Front() const { return m_pData[0]; }

        void Resize(int newSize)
        {
            if (newSize > m_nCapacity)
            {
                Reserve(m_nCapacity << 1);
            }
            m_nSize = newSize;
        }

        // Vector容量扩容
        void Reserve(int newCapacity)
        {
            if (m_nCapacity == 0 && newCapacity == 0)
            {   // 当前尾空列表，至少扩容1个位置
                newCapacity = 1;
            }
            if (m_nCapacity >= newCapacity)
            {   // 当前容量大于目标容量，忽略
                return;
            }
            // 重新申请满足容量的
            value_type* pNewData = nullptr;
            if (m_pData != nullptr)
            {   // 原有数据，使用realloc重新申请满足容量的空间，如有需要，会自动拷贝数据到新的地址上
                pNewData = static_cast<value_type*>(std::realloc(m_pData, static_cast<size_t>(newCapacity) * sizeof(value_type)));
            }
            else
            {   // 第一次申请，使用malloc直接申请
                pNewData = static_cast<value_type*>(std::malloc(static_cast<size_t>(newCapacity) * sizeof(value_type)));
            }
            // 记录内存起始地址和容量，扩容有效数据量是不变的
            m_pData     = pNewData;
            m_nCapacity = newCapacity;
        }

    public:
        // 获取Vector起始位置迭代器
        Iterator Begin() const { return Iterator(m_pData); }
        // 获取Vector尾迭代器
        Iterator End() const { return Iterator(m_pData + m_nSize); }
        // 指定位置后添加元素
        Iterator Insert(const Iterator& pos, const value_type& value)
        {
            if (m_nSize == m_nCapacity)
            { // 容量已满，扩容
                Reserve(m_nCapacity << 1);
            }
            
            for(auto iter = End(); iter != pos; --iter)
            { // 从最后一个元素开始到指定插入位置的所有元素，向后移一位
                (*iter) = (*iter - 1);
            }
        }
        // 指定位置移除元素
        Iterator Erase(const Iterator& iter)
        {
        }

    public:
        /// <summary>
        /// 重载操作符[]获取索引index的数据，常量版本
        /// </summary>
        /// <param name="index">索引位置</param>
        /// <returns></returns>
        const value_type& operator[](int index) const
        {
            value_type* pCurData = m_pData + index;
            return *pCurData;
        }
        /// <summary>
        /// 重载操作符[]获取索引index的数据，可赋值版本
        /// </summary>
        /// <param name="index">索引位置</param>
        /// <returns></returns>
        value_type& operator[](int index)
        {
            value_type* pCurData = m_pData + index;
            return *pCurData;
        }

        const JoyVector& operator=(const JoyVector& rhs)
        {
            if (this != &rhs)
            {
                // 清空旧向量数据
                if (m_pData != nullptr)
                {
                    delete[] m_pData;
                }
                // 从输入向量中拷贝数据
                m_nCapacity = rhs.Capacity();
                m_nSize     = rhs.Size();
                m_pData     = new value_type[m_nCapacity];
                memcpy(m_pData, rhs.m_pData, m_nSize);
            }
            return *this;
        }

        const JoyVector& operator==(JoyVector&& rhs) noexcept
        {
            if (this == &rhs)
            {
                return *this;
            }
            // 移动控制权
            m_nCapacity = rhs.Capacity();
            m_nSize     = rhs.Size();
            m_pData     = rhs.m_pData;
            // 清空源
            rhs.m_pData     = nullptr;
            rhs.m_nSize     = 0;
            rhs.m_nCapacity = 0;
            return *this;
        }

    private:
        /// <summary>
        /// vector的容量
        /// </summary>
        int m_nCapacity{0};

        /// <summary>
        /// vector中的数据数量
        /// </summary>
        int m_nSize{0};

        /// <summary>
        /// 数据头指针
        /// </summary>
        value_type* m_pData{nullptr};
    };
}   // namespace Joy