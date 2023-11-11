
#pragma once

#include <cstdlib>
#include <cstdint>

/// <summary>
/// 输出内存流
/// </summary>
class OutputMemoryStream
{
public:
	OutputMemoryStream()
		: m_Buffer(nullptr)
		, m_Head(0)
		, m_Capacity(0)
	{
		ReallocBuffer(32);
	}

	~OutputMemoryStream()
	{
		std::free(m_Buffer);
	}

	const char* GetBufferPtr() const { return m_Buffer; }
	uint32_t GetLength() const { return m_Head; }

	void Write(const void* inData, size_t inByteCount);
	void Write(uint32_t inData) { Write(&inData, sizeof(inData)); }
	void Write(int32_t inData) { Write(&inData, sizeof(inData)); }

private:
	void ReallocBuffer(uint32_t inNewLength);

private:
	/// <summary>
	/// 内存流指针
	/// </summary>
	char* m_Buffer;

	/// <summary>
	/// 流的可写入起始位置
	/// </summary>
	uint32_t m_Head;
	
	/// <summary>
	/// 流的总长度
	/// </summary>
	uint32_t m_Capacity;
};

class InputMemoryStream
{
public:
	InputMemoryStream(char* inBuffer, uint32_t inByteCount)
		: m_Buffer(inBuffer)
		, m_Capacity(inByteCount)
		, m_Head(0)
	{
	}

	uint32_t GetRemainingDataSize() const { return m_Capacity - m_Head; }

	void Read(void* outData, uint32_t inByteSize);
	void Read(uint32_t& outData) { Read(&outData, sizeof(outData)); }
	void Read(int32_t& outData) { Read(&outData, sizeof(outData)); }

private:
	char* m_Buffer;
	uint32_t m_Head;
	uint32_t m_Capacity;
};