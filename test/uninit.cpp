#include "../fixx/uninit.hpp"
#include "../fixx/buf.hpp"

namespace {
	using fixx::Uninit;

	static_assert(std::is_trivially_constructible_v<Uninit<int>>);
	static_assert(std::is_trivially_destructible_v<Uninit<int>>);

	static_assert([u = Uninit<bool>{}]() mutable { return u.emplace(true); }());
	static_assert([u = Uninit<int>{}] { return std::to_address(u); }());
	static_assert([u = Uninit<fixx::Buf<int>>{}]() mutable {
		return u.emplace(42), u.reset(), true;
	}());
	static_assert([u = Uninit<fixx::Buf<int>>{}]() mutable {
		auto const _3 = u.emplace(3).size(), _42 = u.reset().emplace(42).size();
		return u.reset(), _3 == 3 && _42 == 42;
	}());
}