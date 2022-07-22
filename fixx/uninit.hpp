#ifndef FIXX_HEADER_UNINIT
#define FIXX_HEADER_UNINIT

#include "core.hpp"
#include <memory>

namespace fixx {
	template<typename T> class Uninit {
		struct NoCtor { [[no_unique_address]] T t; };
		union Ctor {
			[[no_unique_address]] std::in_place_t _{};
			[[no_unique_address]] T t;
			
			constexpr Ctor() {}
			constexpr ~Ctor() requires std::is_trivially_destructible_v<T> = default;
			constexpr ~Ctor() requires (!std::is_trivially_destructible_v<T>) {}
		};
		[[no_unique_address]]
		std::conditional_t<std::is_trivially_constructible_v<T>, NoCtor, Ctor> s;
	
	public:
		template<typename... As> constexpr auto& emplace(As&&... as) &
		{ return *std::construct_at(std::addressof(s.t), std::forward<As>(as)...); }
		constexpr auto& reset() & { return s.t.~T(), *this; }
		
		constexpr auto& operator*() const& { return s.t; }
		constexpr auto& operator*() & { return s.t; }
		auto operator*() && = delete;

		constexpr auto operator->() const& { return std::addressof(s.t); }
		constexpr auto operator->() & { return std::addressof(s.t); }
		auto operator->() && = delete;
	};
}

#endif