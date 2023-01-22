/**
 * JoyList.cpp
 * 自定义List数据结构，双向链表
 */

#pragma once

template<typename T> class JoyList
{
private:
    // 链表的节点
    struct JoyListNode
    {
        // 前一个节点
        JoyListNode* Previous{nullptr};
        // 后一个节点
        JoyListNode* Next{nullptr};
        // 节点数据
        T Data{};
        
        JoyListNode(const T& data = T(), JoyListNode* pPrev = nullptr, JoyListNode* pNext = nullptr)
            : Data(data)
            , Previous(pPrev)
            , Next(pNext)
        {
            
        }
    };
    
public:
    // 不可变迭代器
    class Const_Iterator
    {
    public:
        Const_Iterator()
            : m_pCurrent(nullptr)
        {}
        
    public:
        const T& operator* (const Const_Iterator& iter) const
        {
            return this->Retrieve();
        }
        // 前置++运算符重载
        Const_Iterator& operator++()
        {
            m_pCurrent = m_pCurrent->Next;
            return *this;
        }
        // 后置++运算符重载
        Const_Iterator operator++(int)
        {
            Const_Iterator temp = *this;
            ++(*this);
            return temp;
        }
        
        bool operator==(const Const_Iterator& iter) const
        {
            return m_pCurrent == iter.m_pCurrent;
        }
        
        bool operator!=(const Const_Iterator& iter) const
        {
            return !(*this == iter);
        }
        
    protected:
        T& Retrieve() const
        {
            return m_pCurrent->Data;
        }
        
    protected:
        JoyListNode* m_pCurrent;
    };
    // 可变迭代器
    class Iterator : public Const_Iterator
    {
        
    };
    
public:
    JoyList()
    {
        
    }
    JoyList(const JoyList& rhs)
    {
        
    }
    ~JoyList()
    {
        
    }
    
    JoyList& operator=(const JoyList& rhs)
    {
        
    }
    
public:
    inline int Size() const { return m_Size; }
    inline bool Empty() const { return Size()() == 0; }
    
    void Clear()
    {
        
    }
    
    void Push_Back(const T& data)
    {
        
    }
    
    void Push_Front(const T& data)
    {
        
    }
    
    const T& Back() const
    {
        return m_pHeader->data;
    }
    
    const T& Front() const
    {
        return m_pTail->data;
    }
    
public:
    Const_Iterator Begin() const
    {
        Const_Iterator iter(m_pHeader);
        return iter;
    }
    
private:
    // 链表头
    JoyListNode* m_pHeader{nullptr};
    // 链表尾
    JoyListNode* m_pTail{nullptr};
    // 链表大小
    int m_Size{0};
};
