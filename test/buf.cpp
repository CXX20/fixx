#include "../fixx/buf.hpp"

namespace {
	using fixx::Buf, fixx::operator""_c;

	using Buf42 = decltype(Buf{fixx::type<int>, 42_c});
	static_assert(std::is_same_v<Buf42, Buf<int, fixx::Value<42>>>);
	static_assert(Buf42::size().value == 42);
	static_assert([buf = Buf42{42_c}]() mutable { return buf.data(); }());
	static_assert((Buf42{42_c}, true), "memory leak");

	using BufN = decltype(Buf<Buf42>{123});
	static_assert(std::is_same_v<BufN, Buf<Buf42, std::size_t>>);
	static_assert(BufN{123}.size() == 123);
	static_assert([buf = BufN{42}]() mutable { return buf.data(); }());
}