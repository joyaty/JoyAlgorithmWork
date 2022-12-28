/**
 * 自定义ADT - 向量
 * 实现STL中vector
 */

#pragma once

/// <summary>
/// 自定义vector
/// </summary>
/// <typeparam name="T"></typeparam>
template<typename T> class JoyVector
{
public:
	/// <summary>
	/// 获取向量容量
	/// </summary>
	size_t Capacity() const { return m_Capacity; }
	/// <summary>
	/// 获取向量
	/// </summary>
	size_t Size() const { return m_Size; }
	/// <summary>
	/// 向量是否为空
	/// </summary>
	bool Empty() const { return Size() == 0; }

public:
	/// <summary>
	/// 重载操作符[]获取索引index的数据，常量版本
	/// </summary>
	/// <param name="index">索引位置</param>
	/// <returns></returns>
	const T& operator[] (int index) const
	{
		T* pCurData = m_Data + index;
		return &pCurData;
	}
	/// <summary>
	/// 重载操作符[]获取索引index的数据，可赋值版本
	/// </summary>
	/// <param name="index">索引位置</param>
	/// <returns></returns>
	T& operator[] (int index)
	{
		T* pCurData = m_Data + index;
		return &pCurData;
	}

private:
	/// <summary>
	/// vector的容量
	/// </summary>
	size_t m_Capacity;

	/// <summary>
	/// vector中的数据数量
	/// </summary>
	size_t m_Size{ 0 };

	/// <summary>
	/// 数据头指针
	/// </summary>
	T* m_Data{ nullptr };
};