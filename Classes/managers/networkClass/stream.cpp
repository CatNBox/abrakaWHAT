#include "stream.h"
#include <algorithm>
#include <cstdlib>

outputMemoryStream::outputMemoryStream()
	:buffer(nullptr), bufHead(0), bufCapacity(0)
{
	reallocBuffer(32);
}

outputMemoryStream::~outputMemoryStream()
{
	std::free(buffer);
}

const char * outputMemoryStream::getBufferPtr() const
{
	return buffer;
}

uint32_t outputMemoryStream::getLength() const
{
	return bufHead;
}

void outputMemoryStream::write(const void * rawData, size_t byteCnt)
{
	//reserve enough capacity
	uint32_t resultHead = bufHead + static_cast<uint32_t>(byteCnt);
	if (resultHead > bufCapacity)
		reallocBuffer(std::max(bufCapacity * 2, resultHead));

	std::memcpy(buffer + bufHead, rawData, byteCnt);

	//progress bufHead for next writing
	bufHead = resultHead;
}

void outputMemoryStream::reallocBuffer(uint32_t newLength)
{
	buffer = static_cast<char*>(std::realloc(buffer, newLength));
	//realloc fail
	if (buffer == nullptr)
	{

	}

	bufCapacity = newLength;
}

inputMemoryStream::inputMemoryStream()
	:bufHead(0), bufCapacity(0)
{
}

inputMemoryStream::~inputMemoryStream()
{
	std::free(buffer);
}

uint32_t inputMemoryStream::getRemainingDataSize() const
{
	return bufCapacity - bufHead;
}

void inputMemoryStream::read(void * recvData, uint32_t buteCnt)
{
}
