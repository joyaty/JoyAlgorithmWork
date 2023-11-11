/**
 * 自定义大端字节序与小端字节序之间的相互转换
 * 大端字节序：从高位到低位，高位存储在内存低地址中，低位存在内存高地址中。大端字节序符合人的习惯
 * 小端字节序：从高位到低位，高位存储在内存高地址中，低位存在内存低地址中。小端字节序符合计算机的处理逻辑
 * 例如：int a = 0x01FA9E80; // 4字节32位数值
 * 假设变量a的内存地址为：0x01000000 (32位平台)，则变量存储的内存布局
 *             大端序	   小端序
 * 0x01000000 : 0x01   |    0x80
 * 0x01000001 : 0xFA   |    0x9E
 * 0x01000002 : 0x9E   |    0xFA
 * 0x01000003 : 0x80   |    0x01
 **/

#pragma once

#include <cstdint>

/// <summary>
/// 无符号2字节类型字节序转换
/// </summary>
/// <param name="inValue"></param>
/// <returns></returns>
inline uint16_t ByteSwap2(uint16_t inValue)
{
	return (inValue >> 8) | (inValue << 8);
}

/// <summary>
/// 无符号4字节类型字节序转换
/// </summary>
/// <param name="inValue"></param>
/// <returns></returns>
inline uint32_t ByteSwap4(uint32_t inValue)
{
	return (inValue >> 24)
		| ((inValue >> 8) & 0x0000ff00)
		| ((inValue << 8) & 0x00ff0000)
		| (inValue << 24);
}

/// <summary>
/// 无符号8字节类型字节序转换
/// </summary>
/// <param name="inValue"></param>
/// <returns></returns>
inline uint64_t ByteSwap8(uint64_t inValue)
{
	return (inValue >> 56)
		| ((inValue >> 40) & 0x000000000000ff00)
		| ((inValue >> 24) & 0x0000000000ff0000)
		| ((inValue >> 8) & 0x00000000ff000000)
		| ((inValue << 8) & 0x000000ff00000000)
		| ((inValue << 24) & 0x0000ff0000000000)
		| ((inValue << 40) & 0x00ff000000000000)
		| (inValue << 56);
}

/// <summary>
/// 通过一个union共享内存，实现同大小的两个类型解释一个变量
/// </summary>
/// <typeparam name="TFromType"></typeparam>
/// <typeparam name="TToType"></typeparam>
template <typename TFromType, typename TToType> 
class TypeAliaser
{
public:
	TypeAliaser(TFromType inFromTypeValue)
		: m_AsFromType(inFromTypeValue)
	{
	}

	TToType& Get() { return m_AsToType; }

	union 
	{
		TFromType m_AsFromType;
		TToType m_AsToType;
	};
};

// 字节序转换模版类
template <typename T, size_t tSize> class ByteSwapper;

/// <summary>
/// 模版特例化 - 二字节类型字节序转换
/// </summary>
/// <typeparam name="T"></typeparam>
template <typename T> class ByteSwapper<T, 2>
{
public:
	T Swap(T inData) const
	{
		uint16_t result = ByteSwap2(TypeAliaser<T, uint16_t>(inData).Get());
		return TypeAliaser<uint16_t, T>(result).Get();
	}
};

/// <summary>
/// 模版特例化 - 四字节类型字节序转换
/// </summary>
/// <typeparam name="T"></typeparam>
template <typename T> class ByteSwapper<T, 4>
{
public:
	T Swap(T inData) const
	{
		uint32_t result = ByteSwap4(TypeAliaser<T, uint32_t>(inData).Get());
		return TypeAliaser<uint32_t, T>(result).Get();
	}
};

/// <summary>
/// 模版特例化 - 八字节类型字节序转换
/// </summary>
/// <typeparam name="T"></typeparam>
template <typename T> class ByteSwapper<T, 8>
{
public:
	T Swap(T inData) const
	{
		uint32_t result = ByteSwap8(TypeAliaser<T, uint64_t>(inData).Get());
		return TypeAliaser<T, uint64_t>(result).Get();
	}
};