#pragma once

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <vector>

#include "memory_stream.h"

namespace twinkle
{
	typedef std::vector<std::byte> ByteBuffer;

	class ByteStream
	{
	public:

		ByteStream() : m_buffer{} {}
		ByteStream(const ByteBuffer& buffer) : m_buffer{ buffer } {}

		inline const ByteBuffer& getBuffer() const { return m_buffer; }
		inline const std::byte* getNativeBuffer() const { return m_buffer.data(); }

	protected:

		// buffer of bytes
		ByteBuffer m_buffer;
	};

	// The OutputByteBuffer wraps the ByteBuffer.
	// It has methods for writing sequentially into the buffer.

	class OutputByteStream : public ByteStream
	{
	public:

		OutputByteStream() : ByteStream() {}

		template<typename T>
		OutputByteStream& operator<< (const T data)
		{
			static_assert(std::is_fundamental<T>::value || std::is_enum<T>::value,
				"Generic write only supports primitive data type");

			const std::byte* begin = reinterpret_cast<const std::byte*>(std::addressof(data));
			const std::byte* end = begin + sizeof(T);

			m_buffer.insert(m_buffer.end(), begin, end);

			return *this;
		}		
	};

	// The InputByteStream has methods for reading sequentially from the buffer. 

	class InputByteStream : public ByteStream
	{
	public:

		InputByteStream(const ByteBuffer& buffer) : ByteStream(buffer), m_index{ 0 }
		{

		}

		template<typename T>
		InputByteStream& operator>> (T& data)
		{
			static_assert(std::is_fundamental<T>::value || std::is_enum<T>::value,
				"Generic read only supports primitive data type");

			const std::size_t amount = std::min<std::size_t>(sizeof(T), getSize());
			if (amount > 0)
			{
				std::memcpy(&data, &m_buffer[m_index], amount);
				m_index += amount;
			}

			return *this;
		}

		inline std::size_t getSize() const { return m_buffer.size() - m_index; }
		
	private:

		// reading byte index
		uint32_t m_index;
	};
}
