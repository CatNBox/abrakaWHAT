#pragma once
#include <stdint.h>

class outputMemoryStream
{
public:
	outputMemoryStream();
	~outputMemoryStream();

	const char* getBufferPtr() const;
	uint32_t getLength() const;

	void write(const void* rawData, size_t byteCnt);

	template<typename T>
	void write(const T& rawData);

private:
	void reallocBuffer(uint32_t newLength);

	char* buffer;
	uint32_t bufHead;
	uint32_t bufCapacity;
};

template<typename T>
inline void outputMemoryStream::write(const T & rawData)
{
	static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value, "only supports primitive data type");

	write(&rawData, sizeof(rawData));
}

class inputMemoryStream
{
public:
	inputMemoryStream();
	~inputMemoryStream();

	uint32_t getRemainingDataSize() const;

	void read(void* recvData, uint32_t buteCnt);

private:
	char* buffer;
	uint32_t bufHead;
	uint32_t bufCapacity;
};