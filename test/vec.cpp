#include "../fixx/vec.hpp"
#include "../fixx/buf.hpp"

namespace {
	using fixx::Vec, fixx::operator""_c;

	static_assert(std::is_same_v<
			decltype(Vec{std::declval<fixx::Buf<int>&>()}), Vec<int>>);

	static_assert(!Vec<int>{}.size());
	static_assert(!Vec<int>{}.begin());
	static_assert(!Vec<int>{}.end());

	static_assert([] {
		fixx::Buf b{fixx::type<int>, 2_c};
		Vec v{b};
		if (v.size()) return false;
		if (v.emplace_back(42) != 42 || v.size() != 1) return false;
		if (v.emplace_back(53) != 53 || v.size() != 2) return false;
		return v[std::size_t{}] == 42 && v[1_c] == 53;
	}());

	static_assert([] {
		fixx::Buf<int> b{42};
		Vec v{b};
		v.emplace_back(13);
		Vec w{v};
		return w.size() == 1 && w[0] == 13;
	}());
}