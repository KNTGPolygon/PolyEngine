#include "CorePCH.hpp"

#include "BinaryBuffer.hpp"

using namespace Poly;

BinaryBuffer::BinaryBuffer(size_t size) 
	: Size(size)
{
	HEAVY_ASSERTE(size > 0, "Creating buffer with size == 0!");
	Data = (char*)DefaultAlloc(size);
	HEAVY_ASSERTE(Data, "Couldn't allocate memory!");
}

BinaryBuffer::~BinaryBuffer()
{
	if(Data)
		DefaultFree(Data);
}

BinaryBuffer& Poly::BinaryBuffer::operator=(const BinaryBuffer& rhs)
{
	if(Data)
		DefaultFree(Data);
	Data = (char*)DefaultAlloc(rhs.Size);
	memcpy(Data, rhs.Data, rhs.Size);
	Size = rhs.Size;
	return *this;
}

BinaryBuffer& Poly::BinaryBuffer::operator=(BinaryBuffer&& rhs)
{
	if (Data)
		DefaultFree(Data);
	Data = rhs.Data;
	Size = rhs.Size;
	rhs.Data = nullptr;
	rhs.Size = 0;
	return *this;
}
