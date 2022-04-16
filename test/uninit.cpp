#include "../fixx/uninit.hpp"
#include "../fixx/buf.hpp"

namespace {
	using fixx::Uninit;

	static_assert([u = Uninit<int>{}]() mutable { return u.emplace(7) == 7; }());
	static_assert([u = Uninit<int>{}] { return std::to_address(u); }());
	static_assert((Uninit<int>{}, true));
	static_assert((Uninit<fixx::Buf<int>>{}, true));
	static_assert([u = Uninit<int>{}]() mutable { return u.emplace(42); }());
	static_assert([u = Uninit<fixx::Buf<int>>{}]() mutable {
		return u.emplace(42), std::destroy_at(std::to_address(u)), true;
	}());
}