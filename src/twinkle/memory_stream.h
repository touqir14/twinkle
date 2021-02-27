#pragma once 

#include <cstddef>
#include <memory>
#include <vector>

/*
	Many people write bitpackers that work at the byte level. 
	This means they flush bytes to memory as they are filled. 
	This is simpler to code, but the ideal is to read and write words at a time, 
	because modern machines are optimized to work this way instead of farting across a buffer 
	at byte level like it’s 1985.
	If you want to write 32 bits at a time, you’ll need a scratch word twice that size, eg. uint64_t,
	for caching reasons.
*/

namespace twinkle
{
	typedef std::vector<uint32_t> MemoryBuffer;

	class MemoryStream
	{
	public:

		MemoryStream() : m_buffer() {}
		MemoryStream(const std::size_t size) : m_buffer(size) {}
		MemoryStream(const MemoryBuffer& buffer) : m_buffer{ buffer } {}
		MemoryStream(const MemoryStream& stream) = delete;
		~MemoryStream() = default;

		inline const MemoryBuffer& getBuffer() const { return m_buffer; }
		inline const uint32_t* getData() const { return m_buffer.data(); }

		inline std::size_t getSize() const { return m_buffer.size(); }
		inline std::size_t getBitsNum() const { return getSize() * bits_per_word; }

	protected:

		// buffer of bytes
		MemoryBuffer m_buffer;
		// word index
		uint32_t m_wordIndex{ 0 };

		static constexpr std::size_t bits_per_byte = 8;
		static constexpr std::size_t bits_per_word = sizeof(uint32_t) * bits_per_byte;

	};

	class OutputMemoryStream : public MemoryStream
	{
	public:

		OutputMemoryStream() : MemoryStream(1) {}
		OutputMemoryStream(const std::size_t size) : MemoryStream(size) {}

		template<typename T>
		void write(T data, const std::size_t bits = sizeof(T) * bits_per_byte)
		{
			static_assert(std::is_fundamental<T>::value || std::is_enum<T>::value,
				"Generic write only supports primitive data type");
			
			// #todo: endianness

			if (constexpr(bits <= bits_per_word))
			{
				m_scratch |= (data << m_offset);
				m_offset += bits;
				if (m_offset >= bits_per_word)
				{
					// automatic truncation
					m_buffer[m_wordIndex] = uint32_t(m_scratch);
					m_scratch >>= bits_per_word;
					m_buffer.push_back(0);
					++m_wordIndex;
					m_offset -= bits_per_word;
				}
			}
			else
			{
				const std::size_t words_count = bits / bits_per_word;
				const std::size_t rest_bits = bits % bits_per_word;

				for (std::size_t i{ 0 }; i < words_count; ++i)
				{
					write(data, bits_per_word);
					data >>= bits_per_word;
				}

				if (rest_bits != 0)
				{
					write(data, rest_bits);
				}
			}
		}

		template<>
		void write(double data, const std::size_t bits)
		{
			const std::size_t words_count = bits / bits_per_word;
			const std::size_t rest_bits = bits % bits_per_word;

			const uint32_t* begin = reinterpret_cast<const uint32_t*>(std::addressof(data));
			const uint32_t* end = begin + words_count;

			const uint32_t* it;

			for (it = begin; it != end; ++it)
			{
				write(*it, bits_per_word);
			}

			if (rest_bits != 0)
			{
				write(*(it + 1), rest_bits);
			}
		}

		void flush()
		{
			if (m_offset > 0)
			{
				assert(m_offset < bits_per_word);
				m_buffer[m_wordIndex] = uint32_t(m_scratch);
				m_scratch = m_offset = 0;
			}
		}

	private:

		// used to store temporary bits
		uint64_t m_scratch{ 0 };
		// scratch bit offset
		std::size_t m_offset{ 0 };
	};

	class InputMemoryStream : public MemoryStream
	{
	public:

		InputMemoryStream(const MemoryBuffer& buffer) : MemoryStream(buffer) 
		{
			m_scratch = m_buffer[m_wordIndex];
			m_offset = bits_per_word;
		}

		template<typename T>
		bool read(T& data, const std::size_t bits = sizeof(T) * bits_per_byte)
		{
			static_assert(std::is_fundamental<T>::value || std::is_enum<T>::value,
				"Generic read only supports primitive data type");

			// #todo: endianness

			if (constexpr(bits <= bits_per_word))
			{
				if (bits > m_offset)
				{
					if (m_wordIndex < getSize() - 1)
					{
						++m_wordIndex;
						m_scratch |= (uint64_t(m_buffer[m_wordIndex]) << m_offset);
						m_offset += bits_per_word;
					}
					else
					{
						if (m_offset == 0)
						{
							// no more data to read
							return false;
						}
					}
				}

				// example, 1 << 3 will produce 1000, 1000 - 1 = 0111
				const uint64_t mask = (one << bits) - 1;
				data = T(m_scratch & mask);
				m_scratch >>= bits;
				m_offset -= bits;
				return true;
			}
			else
			{
				const std::size_t words_count = bits / bits_per_word;
				const std::size_t rest_bits = bits % bits_per_word;
				const std::size_t words_amount = words_count + (rest_bits != 0) ? 1 : 0;

				const std::size_t amount = std::min<std::size_t>(words_amount, getSize() - m_wordIndex);
				if (amount > 0)
				{
					std::memcpy(&data, &m_buffer[m_wordIndex], amount);
					m_wordIndex += amount;

					if (amount == words_amount && rest_bits > 0)
					{
						uint32_t* address = reinterpret_cast<uint32_t*>(std::addressof(data));
						*(address + words_count) &= ((one << (bits_per_word - m_offset)) - 1);
					}

					m_scratch = m_buffer[m_wordIndex] >> m_offset;
					m_offset = bits_per_word - m_offset;
					return true;
				}
				return false;
			}			
		}

	private:

		static constexpr uint64_t one = uint64_t(1);

		// used to store temporary bits
		uint64_t m_scratch;
		// scratch bit offset
		std::size_t m_offset;
	};
}