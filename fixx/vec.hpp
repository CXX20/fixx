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
		constexpr auto& operator[](std::size_t i) const
		{ return constexpr_assert(i < size()), from[i]; }
		template<typename... As> constexpr auto& emplace_back(As&&... args) &
		{ return std::construct_at(to, std::forward<As>(args)...), *to++; }
	};
	template<typename B> Vec(B&& buf)
	-> Vec<std::remove_pointer_t<decltype(buf.data())>>;
}

#endif