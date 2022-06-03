# The `fixx` C++ library

Most of the times C++ is used as either something tricky and unsafe (C-style) or slow and inexpressive (C with classes/Java-style). However, it is none of these when written with the required knowledge and approach.

The `fixx` (fix + CXX) library aims at illustrating and supporting modern idiomatic C++. Thanks to minimalistic implementation, you can always jump into the source code and see how the things work.

## Features

Only the most interesting features are presented in this list. For exhaustiveness, see [the source code](./fixx/).

* [**arr**](./fixx/arr.hpp)

	Unlike `std::array`,
	* infers the contained type from the first initializer argument: `Arr{1L, 2, '3'}` contains `long`s,
	* checks bounds at compile-time whenever possible: neither `Arr{1, 2}[3]` nor `small_arr[12'345_c]` type check,
	* has `static` `size()`.

* [**buf**](./fixx/buf.hpp)

	To perform fewer allocations and RAII moves, create a big `Buf` and use slices of its underlying array whenever dynamic memory is needed.

* [**core**](./fixx/core.hpp)

	`fixx::Pipe`
	* replaces messy `a(b(c(d(e, f)), g))` chaining with clear `e | (d, f) | c | (b, g) | a`,
	* needs no in-class changes (and no empty parentheses!), unlike `e.d(f).c().b(g).a()`,
		* just wrap your functional object: `Pipe constexpr fn = default_fn`,
	* can still be called as usual: `pipe(arg).member` instead of `(arg | pipe).member`.

	Unlike `std::integral_constant`, `fixx::Value`
	* infers the type of its parameter,
	* has a nice shorthand: `42_c` instead of `std::integral_constant<int, 42>{}`,
	* offers "lifted" operators: `42_c == 42_c` returns `value<true>`.

	Unlike `std::type_identity`, `fixx::Type` compares types like values: `type<int> != type<long>` returns `value<true>`.

* [**tuple**](./fixx/tuple.hpp)

	Unlike `std::tuple`,
	* compiles quickly thanks to using no recursion,
	* is an aggregate type, therefore
		* easy to optimize,
		* `&&`/`const&` members provide temporary lifetime extension,
	* can be indexed polymorphically with containers: `either[42_c]`.

* [**uninit**](./fixx/uninit.hpp)

	Fixes placement `new` not being `constexpr`:
	```c++
	Uninit<Type> uninitialized;
	Type& initialized = uninitialized.emplace(arg1, arg2);
	```

## Limitations

Something is impossible/worthless to support. Here's the list of things `fixx` rejects:

* Non-ALL_CAPS macros / foreign macros starting with `FIXX_` / single letter macros

	Defending from them is impossible and pointless anyway. BTW have you seen the naming of `std` locals?

* `const&&`

	`const` means (seemingly?) immutable. `&&` represents a value which can be mutated however you like (because it isn't needed any more). So, these two seem incompatible. Throwing this combination away allows to provide no `const&&` overloads and lifts the "don't `move` `const` values" lint to the type level.

* `noexcept`

	> You have to spell it three times

	...but you don't want to do it even twice. While `return` is obligatory and `requires` can usually be avoided, `noexcept` hardly aids static compilation and punishes mistakes with `std::terminate`, so it is decided to be given up.

* Exceptions while moving

	Non-throwing moves are required for rollbacks to provide strong exception safety.

* `noexcept` destructors

	Even though C++ doesn't want to accept it, RAII destruction is fallible - e.g. writing to a file.

* Some `std` interfaces

	Most `std` traits come from ancient no-`decltype` C++98/C++03 and are just useless boilerplate now. Moreover, using class specialization instead of function overloading requires reopening `namespace std` and writing code in the foreign context.

	Besides, a lot of typical member functions - e.g. `empty`, `swap` or `value` - can be implemented only once as a free function template.

## Install and test

Just download the repository and use its [`/fixx/`](./fixx/) as an include directory. Testing is performed at compile time: to run tests, simpy build the `.cpp` files in [`/test/`](./test/).

## See also

For more information, see [`/doc/`](./doc/).