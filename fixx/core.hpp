#ifndef FIXX_HEADER_CORE
#define FIXX_HEADER_CORE

#include <cassert>
#include <type_traits>
#include <utility>

namespace fixx::_core {
	template<typename F> class Piped {
		[[no_unique_address]] F f;
	public:
		constexpr explicit Piped(F const& src): f{src} {}
		template<typename A> constexpr auto operator,(A&& arg) const {
			auto bind = [&]<typename B, typename... Bs>(B&& b, Bs&&... bs) {
				return f(
						std::forward<B>(b), std::forward<A>(arg), std::forward<Bs>(bs)...);
			};
			return Piped<decltype(bind)>{bind};
		};
		template<typename A> friend constexpr auto operator|(A&& arg, Piped me)
		-> decltype(me.f(std::declval<A>())) { return me.f(std::forward<A>(arg)); }
	};
}
namespace fixx {
	template<typename F> struct Pipe: F, _core::Piped<F> {
		constexpr Pipe(F const& fn): F{fn}, _core::Piped<F>{fn} {}
		using F::operator();
	};

	template<typename... Fs> struct Overload: Fs... { using Fs::operator()...; };
	template<typename... Fs> Overload(Fs...) -> Overload<Fs...>;

	template<typename T> concept Metaval =
		std::is_empty_v<T> && std::is_trivial_v<T>;

	Pipe constexpr move = []<typename T>(T& t) -> T&& {
		static_assert(!std::is_const_v<T>, "don't produce (unexpected) const&&");
		return static_cast<T&&>(t);
	};

	Pipe constexpr constexpr_assert = []<typename T>(T const& t) {
		if constexpr (Metaval<T>) static_assert(T::value);
		else std::is_constant_evaluated() && !t ? throw : assert(t);
	};

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
		template<typename U> constexpr Value<t < U::value> operator<(U) const
		{ return {}; }
		template<typename U> constexpr Value<(t > U::value)> operator>(U) const
		{ return {}; }
		template<typename U> constexpr Value<t <= U::value> operator<=(U) const
		{ return {}; }
		template<typename U> constexpr Value<t >= U::value> operator>=(U) const
		{ return {}; }
		template<typename U> constexpr Value<t <=> U::value> operator<=>(U) const
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