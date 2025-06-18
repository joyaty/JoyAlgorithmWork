
#include "Serialization/SerializationUtils.h"
#include <algorithm>
#include <cstring>

void OutputMemoryStream::ReallocBuffer(uint32_t inNewLength)
{
	char* newBuffer = static_cast<char*>(std::realloc(m_Buffer, inNewLength));
	if (newBuffer == nullptr)
	{
		// TODO 分配失败处理
		return;
	}
	m_Buffer = newBuffer;
	m_Capacity = inNewLength;
}

void OutputMemoryStream::Write(const void* inData, size_t inByteCount)
{
	uint32_t resultHead = m_Head + static_cast<uint32_t>(inByteCount);
	if (resultHead > m_Capacity)
	{
		ReallocBuffer(std::max(m_Capacity * 2, resultHead));
	}
	std::memcpy(m_Buffer + m_Head, inData, inByteCount);
	m_Head = resultHead;
}

void OutputMemoryStream::Reset()
{
	m_Head = 0;
}

// ================================================================================

void InputMemoryStream::Read(void* outData, uint32_t inByteCount)
{
	uint32_t resultHead = m_Head + inByteCount;
	if (resultHead > m_Capacity)
	{
		// TODO 错误，读取长度溢出
		return;
	}
	std::memcpy(outData, m_Buffer + m_Head, inByteCount);
	m_Head = resultHead;
}
