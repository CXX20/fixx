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

* [**vec**](./fixx/vec.hpp)

	`fixx::Vec` is pretty different from `std::vector`: it
	* uses an externally preallocated storage,
		* performs no bound checking on insertions, so `for (..) vec.emplace_back(..)` is zero-cost,
	* is both trivially constructible and destructible,
		* stores only trivially destructible types.

## Install and test

Just download the repository and use its [`/fixx/`](./fixx/) as an include directory. Testing is performed at compile time: to run tests, simpy build the `.cpp` files in [`/test/`](./test/).

## See also

For more information, see [`/doc/`](./doc/).