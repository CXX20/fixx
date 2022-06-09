#ifndef FIXX_HEADER_UNINIT
#define FIXX_HEADER_UNINIT

#include "core.hpp"
#include <memory>

namespace fixx {
	template<typename T> class Uninit {
		union { T rt, *ct{}; };
	
		static constexpr auto is_ct() { return std::is_constant_evaluated(); }

		constexpr auto ptr() const { return is_ct() ? ct : std::addressof(rt); }
		constexpr auto ptr() { return is_ct() ? ct : std::addressof(rt); }

	public:
		constexpr Uninit() { if (is_ct()) ct = std::allocator<T>{}.allocate(1); }
		Uninit(Uninit&&) = delete;
		constexpr ~Uninit() { if (is_ct()) std::allocator<T>{}.deallocate(ct, 1); }

		template<typename... As> constexpr auto& emplace(As&&... args) &
		{ return *std::construct_at(ptr(), std::forward<As>(args)...); }

		constexpr auto& reset() & { return std::destroy_at(ptr()), *this; }

		constexpr auto& operator*() const& { return *ptr(); }
		constexpr auto& operator*() & { return *ptr(); }
		auto operator*() && = delete;

		constexpr auto operator->() const& { return ptr(); }
		constexpr auto operator->() & { return ptr(); }
		auto operator->() && = delete;
	};
	template<typename T, std::size_t n> class Uninit<T[n]> {
		union { T rt[n], *ct{}; };

		static constexpr auto is_ct() { return std::is_constant_evaluated(); }
	public:
		static constexpr auto size() { return value<n>; }

		constexpr Uninit() { if (is_ct()) ct = std::allocator<T>{}.allocate(n); }
		Uninit(Uninit&&) = delete;
		constexpr ~Uninit() { if (is_ct()) std::allocator<T>{}.deallocate(ct, n); }

		template<typename I, typename... As>
		constexpr auto& emplace(I idx, As&&... args) & {
			return constexpr_assert(idx < size()),
				*std::construct_at(data() + idx, std::forward<As>(args)...);
		}

		template<typename I> constexpr auto& erase(I i) &
		{ return constexpr_assert(i < size()), std::destroy_at(data() + i), *this; }

		constexpr auto data() const& { return is_ct() ? ct : rt; }
		constexpr auto data() & { return is_ct() ? ct : rt; }
		auto data() && = delete;
	};
}

#endif