#pragma once
#include <bit>
#include <assert.h>
#include <type_traits>
#include <cstdint>

#define assert_m(exp, msg) assert((void(msg), exp))

namespace sm
{
	template <uint64_t N>
	class BitSet
	{
	private:
		using T = std::conditional_t<
			(N <= 8), uint8_t,
			std::conditional_t<
			(N <= 16), uint16_t,
			std::conditional_t<
			(N <= 32), uint32_t, uint64_t>
			>
		>;

		// Calc the size rounding up
		static constexpr size_t m_Size = (N + sizeof(T) * 8 - 1) / (sizeof(T) * 8);

		static constexpr size_t m_BitsPerBlock = sizeof(T) * 8;

		T data[m_Size] = {};

	private:
		void MaskResult()
		{
			constexpr int remain = N % m_BitsPerBlock;
			constexpr T lastMask = (remain == 0) ? ~T(0) : (T(1) << remain) - 1;
			data[m_Size - 1] &= lastMask;
		}

	public:
		BitSet() = default;
		~BitSet() = default;

		T& operator[](unsigned int index)
		{
			return data[index];
		}

		BitSet& operator &=(const BitSet& other)
		{
			for (size_t i = 0; i < m_Size; ++i)
			{
				data[i] &= other.data[i];
			}

			MaskResult();

			return *this;
		}

		BitSet& operator |=(const BitSet& other)
		{
			for (size_t i = 0; i < m_Size; ++i)
			{
				data[i] |= other.data[i];
			}

			MaskResult();

			return *this;
		}

		BitSet& operator ^=(const BitSet& other)
		{
			for (size_t i = 0; i < m_Size; ++i)
			{
				data[i] ^= other.data[i];
			}

			MaskResult();

			return *this;
		}

		BitSet operator ~()
		{
			BitSet tmp(*this);
			tmp.Flip();
			return tmp;
		}

		inline size_t GetSize() const { return m_Size; }
		inline size_t GetBitsPerBlock() const { return m_BitsPerBlock; }

		inline void Set(unsigned int index, bool value = true)
		{
			assert_m(index < N, "Bitset: out of bounds");

			size_t block = index / m_BitsPerBlock;
			size_t bit = index % m_BitsPerBlock;

			if (value)
			{
				data[block] |= (T(1) << bit);
			}
			else
			{
				data[block] &= ~(T(1) << bit);
			}
		}

		void Clear()
		{
			if constexpr (m_Size == 1)
			{
				data[0] = 0;
				return;
			}

			for (auto& x : data)
			{
				x = 0;
			}
		}

		BitSet& Flip()
		{
			if constexpr (m_Size == 1)
			{
				data[0] = ~data[0];
				MaskResult();
				return *this;
			}

			for (auto& x : data)
			{
				x = ~x;
			}

			MaskResult();

			return *this;
		}

		inline BitSet& Flip(unsigned int index)
		{
			assert_m(index < N, "Bitset: out of bounds");

			size_t block = index / m_BitsPerBlock;
			size_t bit = index % m_BitsPerBlock;

			data[block] ^= (T(1) << bit);

			return *this;
		}

		size_t Count() const
		{
			size_t ret = 0;

			if constexpr (m_Size == 1)
			{
				return std::popcount(data[0]);
			}

			for (auto x : data)
			{
				ret += std::popcount(x);
			}

			return ret;
		}

		size_t Count(unsigned int index) const
		{
			return std::popcount(data[index]);
		}

		inline bool Has(unsigned int index)	 const // std::bitset::Test()
		{
			assert_m(index < N, "Bitset: out of bounds");

			size_t block = index / m_BitsPerBlock;
			size_t bit = index % m_BitsPerBlock;

			return data[block] & (T(1) << bit);
		}

		bool HasAll(const BitSet& mask) const
		{
			assert_m(m_Size == mask.m_Size, "Bitset: mismatch mask");

			if constexpr (m_Size == 1)
			{
				return (data[0] & mask.data[0]) == mask.data[0];
			}

			for (size_t i = 0; i < m_Size; i++)
			{
				if ((data[i] & mask.data[i]) != mask.data[i])
				{
					return false;
				}
			}

			return true;
		}

		bool HasAny(const BitSet& mask) const
		{
			assert_m(m_Size == mask.m_Size, "Bitset: mismatch mask");

			if constexpr (m_Size == 1)
			{
				return (data[0] & mask.data[0]) != 0;
			}

			for (size_t i = 0; i < m_Size; i++)
			{
				if ((data[i] & mask.data[i]) != 0)
				{
					return true;
				}
			}

			return false;
		}
	};
} //namespace sm