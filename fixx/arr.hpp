#ifndef FIXX__HEADER_ARR
#define FIXX__HEADER_ARR

#include "core.hpp"

namespace fixx {
	template<typename T, typename N, typename = void> struct Arr {
		T _raw[N::value];
		
		template<typename I> static constexpr auto typeof(I)
		-> std::enable_if_t<I::value < N::value, Type<T>> { return {}; }
		static constexpr auto size() { return N{}; }
		constexpr auto operator<=>(Arr const&) const = default;
		
		constexpr auto begin() const& { return _raw; }
		constexpr auto begin() & { return _raw; }
		auto begin() && = delete;
		
		constexpr auto end() const& { return _raw + size(); }
		constexpr auto end() & { return _raw + size(); }
		auto end() && = delete;

		template<typename I> constexpr auto&& operator[](I i) const&
		{ return constexpr_assert(i < size()), _raw[i]; }
		template<typename I> constexpr auto&& operator[](I i) &
		{ return constexpr_assert(i < size()), _raw[i]; }
		template<typename I> constexpr auto&& operator[](I i) &&
		{ return constexpr_assert(i < size()), move(_raw[i]); }
	};
	template<typename T, typename N>
	struct Arr<T, N, std::enable_if_t<!N::value>> {
		static constexpr auto size() { return N{}; }
		constexpr auto operator<=>(Arr const&) const = default;

		constexpr T const* begin() const& { return {}; }
		constexpr T* begin() & { return {}; }
		auto begin() && = delete;

		constexpr T const* end() const& { return {}; }
		constexpr T* end() & { return {}; }
		auto end() && = delete;
	};
	template<typename T, typename... Ts> Arr(T, Ts...)
	-> Arr<T, Value<1 + int(sizeof...(Ts))>>;
}

#endif