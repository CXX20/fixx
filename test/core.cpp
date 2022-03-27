#include "../fixx/core.hpp"

namespace {
	using fixx::operator""_c;

	static_assert((0_c).value == 0);
	static_assert((1_c).value == 1);
	static_assert((2_c).value == 2);
	static_assert((12_c).value == 12);
	static_assert((123_c).value == 123);

	static_assert((-123_c).value == -123);
	static_assert((!0_c).value);
	static_assert(!(!1_c).value);

	static_assert((1_c == 1_c).value);
	static_assert((1_c != 2_c).value);

	int constexpr i = 42_c;
	static_assert(i == 42);

	char constexpr c = 42_c;
	static_assert(c == 42);

	using fixx::Type, fixx::type;

	static_assert(std::is_same_v<typename Type<int>::type, int>);
	static_assert((type<int> == type<int>).value);
	static_assert((type<int> != type<void>).value);
}