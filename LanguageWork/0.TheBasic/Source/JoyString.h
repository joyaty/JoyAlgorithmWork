
#include <cstring>
#include <ostream>

namespace Joy
{
    // 封装C分割字符串处理
    class JoyString
    {
    public:
        // 默认构造
        JoyString();
        // C风格原始字符串初始化构造
        JoyString(const char* pSrcString);
        // 拷贝构造
        JoyString(const JoyString& srcString);
        // 移动构造
        JoyString(JoyString&& srcString);
        // 析构
        virtual ~JoyString();

    public:
        // 获取字符串长度
        int Length() const { return m_pRawStr == nullptr ? 0 : strlen(m_pRawStr); }

    public:
        // 重载 == 运算符，判断字符串是否相等
        bool operator==(const JoyString& srcString);
        // 重写赋值运算符 - 拷贝版本
        void operator=(const JoyString& srcString);
        // 重写赋值运算符 - 移动版本
        void operator=(JoyString&& srcString);

    public:
        // 友元函数申明，用于输出
        friend std::ostream& operator<<(std::ostream& os, const JoyString& str);

    private:
        // C风格字符串原始数据
        char* m_pRawStr{nullptr};
        int   m_InstID{0};

    private:
        static int s_InstIDGenerator;
    };

}   // namespace Joy