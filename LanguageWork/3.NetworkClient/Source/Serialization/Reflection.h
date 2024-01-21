/**
 * 实现一个基础的反射系统
 * Reflection.h
 **/

#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <initializer_list>

/// <summary>
/// 枚举原始类型
/// </summary>
enum class EnumPrimitiveType
{
	EPT_Int,
	EPT_Float,
	EPT_String
};

/// <summary>
/// 变量成员
/// </summary>
class MemberVariable
{
public:
	MemberVariable(const char* inName, EnumPrimitiveType inPrimitiveType, uint32_t inOffset)
		: m_Name(inName)
		, m_PrimitiveType(inPrimitiveType)
		, m_Offset(inOffset)
	{
	}

public:
	EnumPrimitiveType GetPrimitiveType() const { return m_PrimitiveType; }
	uint32_t GetOffset() const { return m_Offset; }

private:
	/// <summary>
	/// 变量名
	/// </summary>
	std::string m_Name;
	/// <summary>
	/// 变量类型
	/// </summary>
	EnumPrimitiveType m_PrimitiveType;
	/// <summary>
	/// 变量在类中的内存地址偏移
	/// </summary>
	uint32_t m_Offset;
};

/// <summary>
/// 类型反射，包含一系列变量定义
/// </summary>
class DataType
{
public:
	DataType(const std::initializer_list<MemberVariable>& inMemberVariables)
		: m_MemberVariables(inMemberVariables)
	{
	}

public:
	const std::vector<MemberVariable>& GetMemberVariables() const
	{
		return m_MemberVariables;
	}

private:
	std::vector<MemberVariable> m_MemberVariables;
};

// 计算成员变量mv在类c中的内存偏移量
// 理解：nullptr地址为0x00000000
// 不使用地址指向内存的内容，通过"->"操作符，获取成员，通过取地址符&，获取对应成员的地址
// 转换为size_t类型，即可得到当前地址对应nullptr的偏移，即为成员在类内存中的偏移值
#define OffsetOf(c, mv) ((size_t) & (static_cast<c*>(nullptr)->mv))