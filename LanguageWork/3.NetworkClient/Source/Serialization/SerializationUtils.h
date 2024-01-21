
#pragma once

#include <cstdlib>
#include <cstdint>

// =========================================================================
// OutputMemoryStream

/// <summary>
/// 输出内存流，
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

// =========================================================================
// InputMemoryStream

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

// =========================================================================
// OutputMemoryBitStream

/// <summary>
/// 内存比特输出流，实现按比特将数据写入流，使用尽可能少的比特写入更多的数据
/// </summary>
class OutputMemoryBitStream
{
public:
	OutputMemoryBitStream();
	~OutputMemoryBitStream()
	{
		std::free(m_Buffer);
	}

public:
	/// <summary>
	/// 写入字节数据
	/// </summary>
	/// <param name="inData"></param>
	/// <param name="inByteCount"></param>
	void WriteBytes(const void* inData, size_t inByteCount);

	/// <summary>
	/// 将一个字节的数据写入比特流中
	/// </summary>
	/// <param name="inData">一个字节的数据</param>
	/// <param name="inBitCount">数据的比特长度</param>
	void WriteBits(uint8_t inData, size_t inBitCount);

	/// <summary>
	/// 数据块按单字节写入比特流
	/// </summary>
	/// <param name="inData">数据指针</param>
	/// <param name="inBitCount">数据的比特总长度</param>
	void WriteBits(const void* inData, size_t inBitCount);

public:
	/// <summary>
	/// 获取流缓冲区指针
	/// </summary>
	/// <returns></returns>
	const char* GetBufferPtr() const { return m_Buffer; }

	/// <summary>
	/// 获取当前写入的比特长度
	/// </summary>
	/// <returns></returns>
	uint32_t GetBitLength() const { return m_BitHead; }

	/// <summary>
	/// 获取当前的字节长度
	/// 进一位，除于8
	/// </summary>
	uint32_t GetByteLength() const { return (m_BitHead + 7) >> 3; }

private:
	/// <summary>
	/// 申请流缓冲区空间
	/// </summary>
	/// <param name="inNewBitCapacity"></param>
	void ReallocBuffer(uint32_t inNewBitCapacity);

private:
	/// <summary>
	/// 留缓冲区指针
	/// </summary>
	char* m_Buffer;

	/// <summary>
	/// 当前缓冲区可写入位置，含义为总的写入比特数
	/// 32个比特位，后3个比特为当前写入字节位置的比特位偏移，剩余29位为当前写入缓冲区位置字节偏移
	/// </summary>
	uint32_t m_BitHead;

	/// <summary>
	/// 缓冲区容量
	/// </summary>
	uint32_t m_BitCapacity;
};