# Flags

A simple header-only C++ library for obtaining program options.

# Installation

Because flags is a header-only library, using it is as straightforward as including the single header file.

```cpp
#include "flags/flags.hpp"
```

The suggested way to keep the library up to date, in C++ projects managed by Git, is to add it as a submodule.

```bash
git submodule add https://www.github.com/jwbuurlage/flags.git ext/flags
git submodule update --remote --init
```

If your project is built using CMake, then you can add flags as a dependency:

```cmake
# this will set the required include directories and compile flags
add_subdirectory("ext/flags")

...
add_executable(your_program ...)
target_link_libraries(your_program flags)
```

# Usage

```cpp
#include <iostream>

#include "flags/flags.hpp"

int main(int argc, char** argv) {
    auto options = flags::flags{argc, argv};

    if (!options.required_args("-x")) {
        return -1;
    }

    auto required_x = options.arg_as<int>("-x");
    auto y = options.arg_as_or<int>("-y", 520);
    auto real = options.arg_as_or<float>("-r", 100.0f);
    auto some_mode = options.passed("--mode");
    auto xs = options.args("--xs");

    return 0;
}
```
