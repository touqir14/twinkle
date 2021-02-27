#pragma once

#include "memory_stream.h"

namespace twinkle
{
	class DataWriter
	{
	public:

		enum { isWriting = 1 };
		enum { isReading = 0 };

	private:

		OutputMemoryStream m_stream;
	};

	class DataReader
	{
	public:
		
		enum { isWriting = 0 };
		enum { isReading = 1 };

	private:

		InputMemoryStream m_stream;
	};
}