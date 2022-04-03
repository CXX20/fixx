# The `fixx` C++ library

Most of the times C++ is used as either something tricky and unsafe (C-style) or slow and inexpressive (C with classes/Java-style). However, it is none of these when written with the required knowledge and approach.

The `fixx` (fix + CXX) library aims at illustrating and supporting modern idiomatic C++:
* header-only
	* easy to install
	* better compiler optimizations
	* rich type system
* generic
* zero-cost
* type-safe
* clutter-free
* tooling-friendly
* low compile-time overhead

Thanks to minimalistic implementation, you can always jump into the source code to see how the things work.

## Features

**"Constexpr" parameters**. Even though C++ doesn't allow constexpr function parameters, `fixx` introduces "meta values" (e.g. `value<42>`) and treats them polymorphically with runtime values (e.g. `42`). This approach grants both extra safety (e.g. "constexpr" static array indices are checked at compile time) and extra features (e.g. indexing vectors and tuples polymorphically via the usual `either[42_c]` syntax).

## Install and test

Just download the repository and use its [`/fixx/`](../fixx/) as an include directory. Testing is performed at compile time: to run tests, simpy build `.cpp` files in [`/test/`](../test/).