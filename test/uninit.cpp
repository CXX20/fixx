#include "../fixx/uninit.hpp"
#include "../fixx/buf.hpp"

namespace {
	using fixx::Uninit, fixx::operator""_c;

	static_assert(!std::is_move_constructible_v<Uninit<int>>);
	static_assert(!std::is_move_constructible_v<Uninit<int[42]>>);

	static_assert([u = Uninit<int>{}]() mutable { return u.emplace(7) == 7; }());
	static_assert([u = Uninit<int>{}] { return std::to_address(u); }());
	static_assert((Uninit<int>{}, true));
	static_assert((Uninit<fixx::Buf<int>>{}, true));
	static_assert([u = Uninit<int>{}]() mutable { return u.emplace(42); }());
	static_assert([u = Uninit<fixx::Buf<int>>{}]() mutable {
		return u.emplace(42), u.reset(), true;
	}());

	static_assert(Uninit<int[3]>::size().value == 3);
	static_assert([u = Uninit<int[3]>{}] { return u.data(); }());
	static_assert([u = Uninit<int[3]>{}]() mutable { return u.data()[2] = 7; }());
	static_assert([] {
		Uninit<fixx::Buf<int>[3]> u;
		auto ret = u.emplace(1_c, 42).data();
		return u.erase(1_c), ret;
	}());

	struct Arg { constexpr Arg(int) {} };

	static_assert([u = Uninit<Arg>{}]() mutable { return u.emplace(7), true; }());
}