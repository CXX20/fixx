#ifndef FIXX_HEADER_TUPLE
#define FIXX_HEADER_TUPLE

#include "core.hpp"
#include <compare>

namespace fixx::_tuple {
	template<typename T, std::size_t i> struct Elem {
		[[no_unique_address]] T _t;

		template<typename J> static constexpr auto typeof(J)
		-> std::enable_if_t<J::value == i, Type<T>> { return {}; }
		template<typename U> static constexpr auto indexof(U)
		-> std::enable_if_t<std::is_same_v<typename U::type, T>, Value<i>>
		{ return {}; }

		template<typename J> constexpr auto operator[](J) const&
		-> std::enable_if_t<J::value == i, T const&> { return _t; }
		template<typename J> constexpr auto operator[](J) &
		-> std::enable_if_t<J::value == i, T&> { return _t; }
		template<typename J> constexpr auto operator[](J) &&
		-> std::enable_if_t<J::value == i, T&&> { return std::forward<T>(_t); }

		template<typename U> constexpr auto operator[](U) const&
		-> std::enable_if_t<std::is_same_v<typename U::type, T>, T const&>
		{ return _t; }
		template<typename U> constexpr auto operator[](U) &
		-> std::enable_if_t<std::is_same_v<typename U::type, T>, T&>
		{ return _t; }
		template<typename U> constexpr auto operator[](U) &&
		-> std::enable_if_t<std::is_same_v<typename U::type, T>, T&&>
		{ return std::forward<T>(_t); }

		constexpr auto operator<=>(Elem const&) const = default;
	};

	template<typename...> struct Numbered;
	template<std::size_t... is, typename... Ts>
	struct Numbered<std::index_sequence<is...>, Ts...>: Elem<Ts, is>... {
		using Elem<Ts, is>::typeof...;
		using Elem<Ts, is>::indexof...;
		using Elem<Ts, is>::operator[]...;
		constexpr auto operator<=>(Numbered const&) const = default;
	};
}
namespace fixx {
	template<typename T> auto constexpr tuple_size =
		std::remove_cvref_t<T>::size();
	template<std::size_t i, typename T> using TupleElem =
		typename decltype(std::remove_cvref_t<T>::typeof(value<i>))::type;

	Pipe constexpr apply = [](auto&& fn, auto&& tuple) -> decltype(auto) {
		return [&]<auto... is>(std::index_sequence<is...>) -> decltype(auto) {
			return fn(std::forward<decltype(tuple)>(tuple)[value<is>]...);
		}(std::make_index_sequence<tuple_size<decltype(tuple)>>{});
	};

	template<typename... Ts> class Tuple
	: public _tuple::Numbered<std::index_sequence_for<Ts...>, Ts...> {
		template<typename T, typename U, std::size_t... is, std::size_t... js>
		static constexpr auto cat(
				T&& t, U&& u, std::index_sequence<is...>, std::index_sequence<js...>) {
			return Tuple<Ts..., TupleElem<js, U>...>{
					std::forward<T>(t)[value<is>]..., std::forward<U>(u)[value<js>]...};
		}
		template<typename T, typename U> static constexpr auto cat(T&& t, U&& u) {
			return cat(
					std::forward<T>(t),
					std::forward<U>(u),
					std::make_index_sequence<tuple_size<T>>{},
					std::make_index_sequence<tuple_size<U>>{});
		}

	public:
		static constexpr auto size() { return value<sizeof...(Ts)>; }
		
		template<typename U> constexpr auto operator+(U&& other) const&
		{ return cat(*this, std::forward<U>(other)); }
		template<typename U> constexpr auto operator+(U&& other) &&
		{ return cat(move(*this), std::forward<U>(other)); }
	};
	template<typename... Ts> Tuple(Ts...) -> Tuple<Ts...>;
}
namespace fixx::_tuple {
	template<typename E, typename T> struct ElemAlias { E elem; T type; };
	template<typename E, typename T> ElemAlias(E&&, T) -> ElemAlias<E&&, T>;
	
	Pipe constexpr to_aliases =
		[]<typename T, std::size_t... is>(T&& tuple, std::index_sequence<is...>) {
			return Tuple{ElemAlias{
					std::forward<T>(tuple)[value<is>], type<TupleElem<is, T>>}...};
		};
}
namespace fixx {
	Pipe constexpr tuple_cat = []<typename... Ts>(Ts&&... tuples) {
		return [](auto&&... aliases) {
			return Tuple<typename decltype(aliases.type)::type...>{
					std::forward<decltype(aliases.elem)>(aliases.elem)...};
		} | (apply, (... + _tuple::to_aliases(
				std::forward<Ts>(tuples), std::make_index_sequence<tuple_size<Ts>>{})));
	};
}

#endif