#pragma once

#include "byte_stream.h"

namespace twinkle
{
	class BitStream : public ByteStream
	{
	public:

		BitStream() : ByteStream() {};
		BitStream(const ByteBuffer& buffer) : ByteStream(buffer) {}

	protected:

		// byte counter
		uint32_t m_byteIndex{ 0 };
		// offset counter
		uint8_t m_offset{ 0 };
	};

	class OutputBitStream : public BitStream
	{
	public:

		OutputBitStream() : BitStream() 
		{
			// allocate one empty byte
			m_buffer.push_back({});
		}

		template<typename T>
		void write(const T data, const std::size_t bits = sizeof(T) * 8)
		{
			static constexpr uint8_t bits_per_byte = sizeof(std::byte) * 8;

			static_assert(std::is_fundamental<T>::value || std::is_enum<T>::value,
				"Generic write only supports primitive data type");

			if (m_offset == bits_per_byte)
			{
				m_buffer.push_back({});
				++m_byteIndex;
				m_offset = 0;
			}
			if (bits + m_offset <= bits_per_byte)
			{
				// make sure to write only n defined bits
				const uint8_t amount = bits_per_byte - (uint8_t)bits;
				std::byte data_to_write = (std::byte(data) << amount) >> amount;
				m_buffer[m_byteIndex] |= data_to_write << m_offset;
				m_offset += (uint8_t)bits;			
			}
			else
			{
				const std::byte* begin = reinterpret_cast<const std::byte*>(std::addressof(data));
				const std::byte* end = begin + sizeof(T);

				for (auto it = begin; it != end; ++it)
				{
					m_buffer[m_byteIndex] |= (*it << m_offset);
					m_buffer.push_back({});
					++m_byteIndex;
					if (m_offset != 0)
					{
						const uint8_t amount = bits_per_byte - m_offset;
						m_buffer[m_byteIndex] |= (*it >> amount);
					}						
				}
			}
		}
	};

	class InputBitStream : public BitStream
	{
	public:

		InputBitStream(const ByteBuffer& buffer) : BitStream(buffer) {}

		template<typename T>
		void read(T& data, const std::size_t bits = sizeof(T) * 8)
		{
			static constexpr uint8_t bits_per_byte = sizeof(std::byte) * 8;

			static_assert(std::is_fundamental<T>::value || std::is_enum<T>::value,
				"Generic read only supports primitive data type");

			if (m_offset == bits_per_byte)
			{
				++m_byteIndex;
				m_offset = 0;
			}
			if (true) // amount of bits
			{
				if (bits + m_offset <= bits_per_byte)
				{
					const uint8_t amount = bits_per_byte - (uint8_t)bits;
					std::byte in_data = (m_buffer[m_byteIndex] << amount) >> amount;
					std::memcpy(&data, &in_data, sizeof(std::byte));
					m_offset += (uint8_t)bits;
				}
				else
				{
					
				}
			}
		}
	};
}