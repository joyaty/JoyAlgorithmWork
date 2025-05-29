
#include "JoyString.h"
#include <iostream>

namespace Joy
{
    int JoyString::s_InstIDGenerator = 0;

    JoyString::JoyString()
        : m_pRawStr(nullptr)
    {
        m_InstID = ++s_InstIDGenerator;
        std::cout << "Constractor JoyString --" << m_InstID << std::endl;
    }

    JoyString::JoyString(const char* pSrcString)
        : Joy::JoyString()
    {
        m_pRawStr = new char[strlen(pSrcString) + 1];
        strcpy(m_pRawStr, pSrcString);
    }

    JoyString::JoyString(const JoyString& srcString)
        : Joy::JoyString()
    {
        m_pRawStr = new char(srcString.Length() + 1);
        strcpy(m_pRawStr, srcString.m_pRawStr);
    }

    JoyString::JoyString(JoyString&& srcString)
        : Joy::JoyString()
    {
        this->m_pRawStr     = srcString.m_pRawStr;
        srcString.m_pRawStr = nullptr;
    }

    JoyString::~JoyString()
    {
        std::cout << "JoyString::~JoyString -- " << m_InstID << std::endl;
        delete[] m_pRawStr;
        m_pRawStr = nullptr;
    }

    bool JoyString::operator==(const JoyString& srcString)
    {
        if (this->m_pRawStr == nullptr && srcString.m_pRawStr == nullptr)
        {
            return true;
        }
        if (this->m_pRawStr == nullptr || srcString.m_pRawStr == nullptr)
        {
            return false;
        }
        return strcmp(this->m_pRawStr, srcString.m_pRawStr) == 0;
    }

    void JoyString::operator=(const JoyString& srcString)
    {
        if (*this == srcString)
        {
            return;
        }

        if (this->m_pRawStr != nullptr)
        {   // 清理原数据
            delete[] this->m_pRawStr;
            this->m_pRawStr = nullptr;
        }
        // 拷贝目标字符串数据
        this->m_pRawStr = new char(srcString.Length() + 1);
        strcpy(m_pRawStr, srcString.m_pRawStr);
    }

    void JoyString::operator=(JoyString&& srcString)
    {
        if (*this == srcString)
        {
            srcString.m_pRawStr = nullptr;
            return;
        }
        if (this->m_pRawStr != nullptr)
        {   // 清理原数据
            delete[] this->m_pRawStr;
            this->m_pRawStr = nullptr;
        }
        this->m_pRawStr     = srcString.m_pRawStr;
        srcString.m_pRawStr = nullptr;
    }

    std::ostream& operator<<(std::ostream& os, const JoyString& str)
    {
        if (str.m_pRawStr)
        {
            os << str.m_pRawStr;
        }
        return os;
    }
}   // namespace Joy

#pragma region JoyString 单元测试

using Joy::JoyString;

void UnitTest_JoyString()
{
    {
        JoyString str1;
        std::cout << "str1 = " << str1 << ", str1 length = " << str1.Length() << std::endl;

        JoyString str2("hello JoyString");
        JoyString str3(str2);
        std::cout << "str2 = " << str2 << ", str2 length = " << str2.Length() << std::endl;
        std::cout << "str3 = " << str3 << ", str3 length = " << str3.Length() << std::endl;

        JoyString str4(std::move(str2));
        std::cout << "str2 = " << str2 << ", str2 length = " << str2.Length() << std::endl;
        std::cout << "str4 = " << str4 << ", str4 length = " << str4.Length() << std::endl;

        JoyString str5;
        str5 = str3;
        std::cout << "str3 = " << str3 << ", str3 length = " << str3.Length() << std::endl;
        std::cout << "str5 = " << str5 << ", str5 length = " << str5.Length() << std::endl;

        JoyString str6;
        str6 = std::move(str3);
        std::cout << "str3 = " << str3 << ", str3 length = " << str3.Length() << std::endl;
        std::cout << "str6 = " << str6 << ", str6 length = " << str6.Length() << std::endl;
    }
}

#pragma endregion