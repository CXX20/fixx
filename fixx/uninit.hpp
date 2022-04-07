#ifndef FIXX__HEADER_UNINIT
#define FIXX__HEADER_UNINIT

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

		constexpr auto& operator*() const& { return *ptr(); }
		constexpr auto& operator*() & { return *ptr(); }
		auto operator*() && = delete;

		constexpr auto operator->() const& { return ptr(); }
		constexpr auto operator->() & { return ptr(); }
		auto operator->() && = delete;
	};
}

#endif