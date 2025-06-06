/**
 * 自定义ADT - 向量
 * 实现STL中vector
 */

#pragma once

#include <cstring>

namespace Joy
{
    /// <summary>
    /// 自定义Vector
    /// </summary>
    /// <typeparam name="T"></typeparam>
    template<typename T> class JoyVector
    {
    public:
        explicit JoyVector(int capacity)
            : m_Capacity(capacity)
            , m_Size(0)
        {
            m_Data = new T[m_Capacity];
        }

        // 拷贝构造函数
        JoyVector(const JoyVector& rhs)
            : m_Capacity(rhs.Capacity())
            , m_Size(rhs.Size())
        {
            operator=(rhs);
        }

        ~JoyVector()
        {
            m_Capacity = 0;
            m_Size     = 0;
            delete[] m_Data;
        }

    public:
        /// <summary>
        /// 获取向量容量
        /// </summary>
        inline int Capacity() const { return m_Capacity; }
        /// <summary>
        /// 获取向量
        /// </summary>
        inline int Size() const { return m_Size; }
        /// <summary>
        /// 向量是否为空
        /// </summary>
        inline bool Empty() const { return Size() == 0; }

        // 清空向量
        inline void Clear() { m_Size = 0; }

        // 向量尾部添加元素
        void Push_Back(const T& value)
        {
            // 容量已满，扩容
            if (m_Size == m_Capacity)
            {
                Reserve(m_Capacity * 2 + 1);
            }
            m_Data[m_Size] = value;
            ++m_Size;
        }

        // 删除尾部元素
        void Pop_Back() { --m_Size; }

        const T& Back() const { return m_Data[m_Size - 1]; }

        const T& Front() const { return m_Data[0]; }

        void Resize(int newSize)
        {
            if (newSize > m_Capacity)
            {
                Reserve(m_Capacity * 2 + 1);
            }
            m_Size = newSize;
        }

        // 重新预定向量的容量
        void Reserve(int newCapacity)
        {
            if (m_Capacity >= newCapacity)
            {
                return;
            }
            // 迁移数据到新的地址
            int* pOldData = m_Data;
            m_Capacity    = newCapacity;
            m_Data        = new T[m_Capacity];
            for (int i = 0; i < m_Size; ++i)
            {
                m_Data[i] = pOldData[i];
            }

            delete[] pOldData;
        }

    public:
        /// <summary>
        /// 重载操作符[]获取索引index的数据，常量版本
        /// </summary>
        /// <param name="index">索引位置</param>
        /// <returns></returns>
        const T& operator[](int index) const
        {
            T* pCurData = m_Data + index;
            return *pCurData;
        }
        /// <summary>
        /// 重载操作符[]获取索引index的数据，可赋值版本
        /// </summary>
        /// <param name="index">索引位置</param>
        /// <returns></returns>
        T& operator[](int index)
        {
            T* pCurData = m_Data + index;
            return *pCurData;
        }

        const JoyVector& operator=(const JoyVector& rhs)
        {
            if (this != &rhs)
            {
                // 清空旧向量数据
                if (m_Data != nullptr)
                {
                    delete[] m_Data;
                }
                // 从输入向量中拷贝数据
                m_Capacity = rhs.Capacity();
                m_Size     = rhs.Size();
                m_Data     = new T[m_Capacity];
                memcpy(m_Data, rhs.m_Data, m_Size);
            }
            return *this;
        }

    private:
        /// <summary>
        /// vector的容量
        /// </summary>
        int m_Capacity{0};

        /// <summary>
        /// vector中的数据数量
        /// </summary>
        int m_Size{0};

        /// <summary>
        /// 数据头指针
        /// </summary>
        T* m_Data{nullptr};
    };
}   // namespace Joy