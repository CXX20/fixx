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

	static_assert(std::is_same_v<
		decltype(Vec<int>{} <=> Vec<int>{}), std::strong_ordering>);
	static_assert(std::is_same_v<
		decltype(Vec<float>{} <=> Vec<float>{}), std::partial_ordering>);
	
	static_assert([] {
		fixx::Buf<float> buf1{42}, buf2{42};
		Vec vec{buf1};
		vec.emplace_back(1.f), vec.emplace_back(2.f);
		Vec copy{vec};
		Vec shorter{buf2};
		Vec different{buf2};
		different.emplace_back(3.f), different.emplace_back(4.f);
		return
			vec == copy && vec <=> copy == std::strong_ordering::equal &&
			vec != shorter && vec <=> shorter == std::strong_ordering::greater &&
			vec != different && vec <=> different == std::strong_ordering::less;
	}());

	struct NoCmp {};
	static_assert(!Vec<NoCmp>{}.size(), "Vec comparison must be SFINAE-friendly");
}