#ifndef FIXX_HEADER_VEC
#define FIXX_HEADER_VEC

#include "core.hpp"
#include <memory>

namespace fixx {
	template<typename T> class Vec {
		static_assert(std::is_trivially_destructible_v<T>);

		T *from{}, *to{};

	public:
		constexpr Vec() {}
		template<typename B> constexpr Vec(B&& buf, decltype(buf.data()) = nullptr)
		: from{std::forward<B>(buf).data()}, to{from} {}

		constexpr auto begin() const { return from; }
		constexpr auto end() const { return to; }
		constexpr auto size() const { return std::size_t(to - from); }
		template<typename... As> constexpr auto& emplace_back(As&&... args) &
		{ return std::construct_at(to, std::forward<As>(args)...), *to++; }
		constexpr auto& operator[](std::size_t i) const
		{ return constexpr_assert(i < size()), from[i]; }
		template<typename U = T, std::void_t<
			decltype(std::declval<U>() == std::declval<U>())
		>* = nullptr> constexpr auto operator==(Vec const& v) const
		{ return compare(v, [](auto&& a, auto&& b) { return a == b; }); }
		template<typename U = T, std::void_t<
			decltype(std::declval<U>() <=> std::declval<U>())
		>* = nullptr> constexpr auto operator<=>(Vec const& v) const
		{ return compare(v, [](auto&& a, auto&& b) { return a <=> b; }); }
	private:
		template<typename F> constexpr auto compare(Vec const& rhs, F&& f) const
		-> std::common_type_t<decltype(f(*to, *to)), decltype(f(size(), size()))> {
			if (auto p = begin(); size() == rhs.size())
				for (auto&& t: rhs) if (*p != t) return f(*p, t); else ++p;
			return f(size(), rhs.size());
		}
	};
	template<typename B> Vec(B&& buf)
	-> Vec<std::remove_pointer_t<decltype(buf.data())>>;
}

#endif