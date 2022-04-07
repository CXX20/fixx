#include "../fixx/uninit.hpp"

namespace {
	using fixx::Uninit;

	static_assert([u = Uninit<int>{}]() mutable { return u.emplace(7) == 7; }());
	static_assert([u = Uninit<int>{}] { return u.operator->(); }());
	static_assert((Uninit<int>{}, true), "consteval allocation error");
}