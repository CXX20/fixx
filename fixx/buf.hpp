#ifndef FIXX__HEADER_BUF
#define FIXX__HEADER_BUF

#include "core.hpp"
#include <memory>

namespace fixx {
	template<typename T, typename N = std::size_t, typename A = std::allocator<T>>
	class Buf {
		T* arr{};
		[[no_unique_address]] N len;
		[[no_unique_address]] A al;
	
	public:
		constexpr Buf(Type<T>, N elems, A const& alloc = {}): Buf{elems, alloc} {}
		constexpr Buf(N n, A const& a): len{n}, al{a} { arr = al.allocate(len); }
		constexpr explicit Buf(N elems): Buf{elems, {}} {}
		Buf(Buf&&) = delete;
		constexpr ~Buf() { al.deallocate(arr, len); }

		template<typename M = N>
		static constexpr auto size(std::enable_if_t<Metaval<M>>* = {})
		{ return N{}; }
		template<typename M = N>
		constexpr auto size(std::enable_if_t<!Metaval<M>>** = {}) const
		{ return len; }

		constexpr auto data() & { return arr; }
	};
}

#endif