#include "../fixx/core.hpp"

namespace {
	using fixx::Pipe;

	Pipe constexpr inc = [](auto i) { return ++i; };
	static_assert(inc(1) == 2);
	static_assert((1 | inc) == 2);

	Pipe constexpr div = [](auto i, auto j) { return i / j; };
	static_assert(div(12, 4) == 3);
	static_assert((12 | (div, 4)) == 3);
	static_assert(!std::is_invocable_v<decltype((div, 4)), int>);

	Pipe constexpr ternary = [](bool b, int i, int j) { return b ? i : j; };
	static_assert(true  | (ternary, 1, 0));
	static_assert(false | (ternary, 0, 1));
	static_assert(!(true  | (ternary, 0, 1)));
	static_assert(!(false | (ternary, 1, 0)));
	static_assert(!std::is_invocable_v<decltype((ternary, 0, 1)), bool>);

	Pipe constexpr variadic = [](auto... ts) { return (... + ts); };
	static_assert(variadic(1, 2, 3) == 6);
	static_assert((1 | (variadic, 2, 3)) == 6);

	struct NoDefaultCtor {
		constexpr explicit NoDefaultCtor(int) {}
		constexpr auto operator()(bool b) const { return b; }
	};
	Pipe constexpr no_default_ctor = NoDefaultCtor{42};
	static_assert(!no_default_ctor(false));
	static_assert(true | no_default_ctor);

	int constexpr foo = 42;
	Pipe constexpr preserve_ref = [](auto&&) -> auto& { return foo; };
	static_assert(std::is_same_v<decltype(preserve_ref(42)), int const&>);
	static_assert(std::is_same_v<decltype(42 | preserve_ref), int const&>);

	Pipe constexpr preserve_fwd = [](int&&, int&&) { return true; };
	static_assert(1 | (preserve_fwd, 2));
	static_assert(!std::is_invocable_v<decltype((preserve_fwd, 2)), int>);

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