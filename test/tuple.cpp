#include "../fixx/tuple.hpp"

namespace {
	using fixx::Tuple, fixx::operator""_c;

	Tuple constexpr iuc{1, 2u, '3'};
	
	using Iuc = decltype(iuc);
	static_assert(Iuc::size().value == 3);
	static_assert(std::is_same_v<Iuc, Tuple<int, unsigned, char> const>);

	static_assert(std::is_same_v<decltype(iuc[0_c]), int const&>);
	static_assert(std::is_same_v<decltype(iuc[1_c]), unsigned const&>);
	static_assert(std::is_same_v<decltype(iuc[2_c]), char const&>);
	static_assert(std::is_same_v<decltype(Tuple{iuc}[0_c]), int&&>);
	static_assert(std::is_same_v<decltype(Tuple{iuc}[1_c]), unsigned&&>);
	static_assert(std::is_same_v<decltype(Tuple{iuc}[2_c]), char&&>);

	static_assert(std::is_same_v<decltype(iuc[fixx::type<int>]), int const&>);
	static_assert(std::is_same_v<decltype(iuc[fixx::type<char>]), char const&>);
	static_assert(std::is_same_v<decltype(Tuple{iuc}[fixx::type<int>]), int&&>);
	static_assert(std::is_same_v<decltype(Tuple{iuc}[fixx::type<char>]), char&&>);

	static_assert([] {
		Tuple<int const&> clval{1};
		Tuple<int&&> rval{2};
		return clval[0_c] == 1 && rval[0_c] == 2;
	}(), "lifetime extension test failed");

	auto constexpr iuciuc = iuc + iuc;
	static_assert(std::is_same_v<
			decltype(iuc + iuc), Tuple<int, unsigned, char, int, unsigned, char>>);
	static_assert(iuciuc[0_c] == 1);
	static_assert(iuciuc[1_c] == 2u);
	static_assert(iuciuc[2_c] == '3');
	static_assert(iuciuc[3_c] == 1);
	static_assert(iuciuc[4_c] == 2u);
	static_assert(iuciuc[5_c] == '3');

	using fixx::TupleElem, fixx::tuple_size;

	static_assert(tuple_size<Iuc>.value == 3);
	static_assert(std::is_same_v<TupleElem<0, Iuc>, int>);
	static_assert(std::is_same_v<TupleElem<1, Iuc>, unsigned>);
	static_assert(std::is_same_v<TupleElem<2, Iuc>, char>);
	static_assert(std::is_same_v<TupleElem<0, Iuc const&>, int>);
	static_assert(std::is_same_v<TupleElem<1, Iuc&>, unsigned>);
	static_assert(std::is_same_v<TupleElem<2, Iuc&&>, char>);

	using fixx::tuple_cat;

	auto constexpr iuciuciuc = tuple_cat(iuc, iuc, iuc);
	static_assert(std::is_same_v<
			decltype(tuple_cat(iuc, iuc, iuc)),
			Tuple<int, unsigned, char, int, unsigned, char, int, unsigned, char>>);
	static_assert(iuciuciuc[0_c] == 1);
	static_assert(iuciuciuc[1_c] == 2u);
	static_assert(iuciuciuc[2_c] == '3');
	static_assert(iuciuciuc[3_c] == 1);
	static_assert(iuciuciuc[4_c] == 2u);
	static_assert(iuciuciuc[5_c] == '3');
	static_assert(iuciuciuc[6_c] == 1);
	static_assert(iuciuciuc[7_c] == 2u);
	static_assert(iuciuciuc[8_c] == '3');

	Tuple<int const&, char const> constexpr quals{42, 'c'};
	static_assert((quals.typeof(0_c) == fixx::type<int const&>).value);
	static_assert((quals.typeof(1_c) == fixx::type<char const>).value);
	static_assert((quals.indexof(fixx::type<int const&>) == 0_c).value);
	static_assert((quals.indexof(fixx::type<char const>) == 1_c).value);

	auto constexpr quals2 = quals + quals;
	static_assert((quals2.typeof(0_c) == fixx::type<int const&>).value);
	static_assert((quals2.typeof(1_c) == fixx::type<char const>).value);
	static_assert((quals2.typeof(2_c) == fixx::type<int const&>).value);
	static_assert((quals2.typeof(3_c) == fixx::type<char const>).value);

	auto constexpr quals3 = tuple_cat(quals, quals, quals);
	static_assert((quals3.typeof(0_c) == fixx::type<int const&>).value);
	static_assert((quals3.typeof(1_c) == fixx::type<char const>).value);
	static_assert((quals3.typeof(2_c) == fixx::type<int const&>).value);
	static_assert((quals3.typeof(3_c) == fixx::type<char const>).value);
	static_assert((quals3.typeof(4_c) == fixx::type<int const&>).value);
	static_assert((quals3.typeof(5_c) == fixx::type<char const>).value);
	
	static_assert(tuple_cat(Tuple<int&&>{42}, Tuple<char&&>{'c'})[0_c] == 42);
	static_assert(tuple_cat(Tuple<int&&>{42}, Tuple<char&&>{'c'})[1_c] == 'c');
	static_assert((Tuple<int&&>{42} + Tuple<char&&>{'c'})[0_c] == 42);
	static_assert((Tuple<int&&>{42} + Tuple<char&&>{'c'})[1_c] == 'c');
	static_assert((Tuple<int&&>::typeof(0_c) == fixx::type<int&&>).value);
	static_assert((Tuple<int&&>::indexof(fixx::type<int&&>) == 0_c).value);
}