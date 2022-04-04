#ifndef FIXX__HEADER_CORE
#define FIXX__HEADER_CORE

#include <cassert>
#include <type_traits>
#include <utility>

namespace fixx {
	template<typename T> concept Metaval =
		std::is_empty_v<T> && std::is_trivial_v<T>;

	auto constexpr move = []<typename T>(T& t) -> T&& {
		static_assert(!std::is_const_v<T>, "don't produce (unexpected) const&&");
		return static_cast<T&&>(t);
	};

	template<typename T> constexpr void constexpr_assert(T const& t)
	{ std::is_constant_evaluated() && !t ? throw : assert(t); }
	template<Metaval T> constexpr void constexpr_assert(T)
	{ static_assert(T::value); }

	template<auto t> struct Value {
		static auto constexpr value = t;
	
		constexpr operator decltype(t)() const { return t; }
		template<typename U, std::void_t<
			decltype(U{t}), std::enable_if_t<std::is_convertible_v<decltype(t), U>>
		>* = nullptr> constexpr operator U() const { return U{t}; }

		template<typename T = Value> constexpr Value<-T::value> operator-() const
		{ return {}; }
		template<typename T = Value> constexpr Value<!T::value> operator!() const
		{ return {}; }

		template<typename U> constexpr Value<t == U::value> operator==(U) const
		{ return {}; }
		template<typename U> constexpr Value<t != U::value> operator!=(U) const
		{ return {}; }
	};
	template<auto t> Value<t> constexpr value;
	template<char... cs> constexpr auto operator""_c() {
		static_assert((... && (cs != '.')), "not integer");
		char constexpr chars[]{cs...};
		static_assert(chars[0] != '0' || sizeof...(cs) == 1, "not decimal");
		return value<[parsed = 0]() mutable {
			return (..., (parsed = parsed * 10 + cs - '0'));
		}()>;
	}

	template<typename T> struct Type {
		using type = T;

		template<typename U> constexpr auto operator==(U) const
		-> Value<std::is_same_v<type, typename U::type>> { return {}; }
		template<typename U> constexpr auto operator!=(U) const
		-> Value<!std::is_same_v<type, typename U::type>> { return {}; }
	};
	template<typename T> Type<T> constexpr type;
}

#endif