#include <iostream>

#include "flags/flags.hpp"

int main(int argc, char** argv) {
    auto options = flags::flags{argc, argv};
    options.info("flags_example", "an example CLI program using flags");

    auto required_x = options.arg_as<int>("-x");
    auto y = options.arg_as_or<int>("-y", 520);
    auto real = options.arg_as_or<float>("-r", 100.0f);
    auto some_mode = options.passed("--mode");
    auto xs = options.args("--xs");

    if (options.passed("-h") || !options.sane()) {
        std::cout << options.usage();
        return options.passed("-h") ? 0 : -1;
    }

    std::cout << real << "\n";
    std::cout << y << "\n";
    std::cout << required_x << "\n";
    std::cout << some_mode << "\n";
    std::cout << "[";
    auto sep = std::string("");
    for (auto x : xs) {
        std::cout << sep << x;
        sep = ", ";
    }
    std::cout << "]\n";

    return 0;
}
