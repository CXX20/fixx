# The `fixx` C++ library codestyle

This document neither is nor intended to be exhaustive. Only unusual decisions which can't be easily found out (whether themselves or their rationale) from the source code are presented here.

## Indentation

Tabs are used for indentation, spaces - for (rarely applied) alignment. Unlike spaces, tabs provide configurable indentation depth and one-press navigation between indentation levels - no need to smash arrow keys!

The line length limit is 80 characters, assuming tabs are 2 spaces wide.

## Namespaces

Library code is in the `fixx` namespace.

Test code is placed in top-level unnamed namespaces to avoid ODR violations.

Library implementation details are in `fixx::_feature_name` rather than a single `fixx::impl` namespace, allowing to isolate other features' details while implementing new ones. Otherwise, some unexpected name clashes would be possible:
```c++
// feature.hpp:
namespace fixx::impl { struct Detail {}; }
namespace fixx { struct Feature: impl::Detail {}; }

// another_feature.hpp:
#include "feature.hpp"
namespace fixx::impl { struct Detail {}; } // redefinition?!
```

The underscore keeps the name at the end when sorted alphabetically, e.g. in IDE completion suggestions.

## Header guards

They are used instead of `#pragma once` even though each target compiler supports the less verbose option, because if the same header is included through different paths, `#pragma once` doesn't save the day. However, header guards don't care where the code comes from, so they stop redefinitions anyway.

Header guards are named as `FIXX_HEADER_FILE_NAME` to keep autocompletion cleaner compared to the usual `LIB_FILE_NAME_HPP` style.

## Outdated techniques

To stop internal tooling crashes, SFINAE hacks are usually used instead of concepts, explicit `template`s - instead of `auto` function parameters. Besides, signature breaking tricks don't affect overload/specialization resolution, allowing easier user-defined specializations due to less concept ambiguity.