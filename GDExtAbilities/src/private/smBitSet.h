//#include <type_traits>
//#include <stdint.h>
//
//namespace sm
//{
//	template <uint64_t N>
//	class BitSet
//	{
//	public:
//		void Set(int pos, bool value);
//
//	public:
//		using T = std::conditional_t<
//			(N <= 8), uint8_t,
//			std::conditional_t<
//			(N <= 16), uint16_t,
//			std::conditional_t<
//			(N <= 32), uint32_t, uint64_t>
//			>
//		>;
//
//		// Calc the size rounding up
//		static constexpr size_t size = (N + sizeof(T) * 8 - 1) / (sizeof(T) * 8);
//
//		T data[size];
//	};
//} //namespace sm
