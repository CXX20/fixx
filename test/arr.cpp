#include "../fixx/arr.hpp"
#include "../fixx/tuple.hpp"

namespace {
	using fixx::Arr, fixx::operator""_c;

	Arr constexpr arr{1L, 2, '3'};
	using Arr3 = std::remove_const_t<decltype(arr)>;
	static_assert(std::is_same_v<Arr3, Arr<long, fixx::Value<3>>>);
	static_assert((Arr3::typeof(0_c) == fixx::type<long>).value);
	static_assert((Arr3::typeof(1_c) == fixx::type<long>).value);
	static_assert((Arr3::typeof(2_c) == fixx::type<long>).value);
	static_assert(Arr3::size().value == 3);
	static_assert(*arr.begin() == 1 && arr.end()[-1] == '3');
	static_assert(std::is_same_v<decltype(*arr.begin()), long const&>);
	static_assert(arr[0_c] == 1 && arr[1_c] == 2 && arr[2_c] == '3');
	static_assert(Arr{arr}[0_c] == 1);
	static_assert(std::is_same_v<decltype(arr[0_c]), long const&>);
	static_assert(std::is_same_v<decltype(Arr{arr}[0_c]), long&&>);
	static_assert(arr <=> arr == std::strong_ordering::equal);

	using Arr0 = fixx::Arr<int, fixx::Value<0>>;
	static_assert(std::is_empty_v<Arr0>);
	static_assert(Arr0::size().value == 0);
	static_assert(Arr0{} <=> Arr0{} == std::strong_ordering::equal);

	static_assert(fixx::tuple_size<Arr3>.value == 3);
	static_assert(std::is_same_v<fixx::TupleElem<0, Arr3>, long>);
	static_assert(std::is_same_v<fixx::TupleElem<1, Arr3>, long>);
	static_assert(std::is_same_v<fixx::TupleElem<2, Arr3>, long>);
}