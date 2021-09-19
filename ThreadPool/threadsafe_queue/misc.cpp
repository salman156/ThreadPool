#include<type_traits>

template<typename T>
constexpr bool check_single_bit(T bits)
{
	static_assert(std::is_integral<T>::value, "Function check_single_bit is callable only for integral types");

	return bits && !(bits & (bits - 1));
}