#ifndef FIXX_HEADER_VEC
#define FIXX_HEADER_VEC

#include "core.hpp"
#include <memory>

namespace fixx {
	template<typename T, typename N = std::size_t> class Vec {
		static_assert(std::is_trivially_destructible_v<T>);

		T* arr;
		std::size_t len{};
		[[no_unique_address]] N cap;

	public:
		template<typename B, typename = decltype(arr = std::declval<B>().data())>
		constexpr Vec(B&& buf)
		: arr{std::forward<B>(buf).data()}, cap{buf.size()} {}

		constexpr auto begin() const { return arr; }
		constexpr auto end() const { return arr + len; }
		constexpr auto size() const { return len; }
		template<typename I> constexpr auto& operator[](I i) const
		{ return constexpr_assert(i < len), constexpr_assert(i < cap), arr[i]; }
		template<typename... As> constexpr auto& emplace_back(As&&... args) & {
			constexpr_assert(len < cap);
			std::construct_at(arr + len, std::forward<As>(args)...);
			return arr[len++];
		}
	};
	template<typename B> Vec(B&& buf)
	-> Vec<std::remove_pointer_t<decltype(buf.data())>, decltype(buf.size())>;
}

#endif