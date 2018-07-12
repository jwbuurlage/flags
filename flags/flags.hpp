#include <algorithm>
#include <sstream>
#include <string>
#include <vector>
#include <utility>
#include <tuple>

namespace flags {

using namespace std::string_literals;

namespace detail {

std::vector<std::string> split(std::string s, std::string delim) {
    std::vector<std::string> result;
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delim)) != std::string::npos) {
        token = s.substr(0, pos);
        result.push_back(token);
        s.erase(0, pos + delim.length());
    }
    result.push_back(s);
    return result;
}

} // namespace detail

struct flags {
    int argc;
    char** argv;

    std::vector<std::tuple<std::string, bool, std::string>> flags_;

    bool passed(std::string flag) {
        return std::find(argv, argv + argc, flag) != (argv + argc);
    }

    std::string arg(std::string flag) {
        flags_.emplace_back(flag, false, ""s);

        auto pos = std::find(argv, argv + argc, flag);
        if (pos == argv + argc || pos + 1 == argv + argc) {
            return "";
        }
        pos++;

        return std::string(*pos);
    }

    std::vector<std::string> args(std::string flag) {
        flags_.emplace_back(flag, false, "");

        auto pos = std::find(argv, argv + argc, flag);
        std::vector<std::string> result;
        if (pos == argv + argc || pos + 1 == argv + argc) {
            return result;
        }
        pos++;
        while (pos != argv + argc && *pos[0] != '-') {
            result.push_back(std::string(*pos));
            pos++;
        }

        return result;
    }

    template <typename T>
    T arg_as(std::string flag) {
        flags_.emplace_back(flag, false, "");

        auto value = std::stringstream(arg(flag));
        T x = {};
        value >> x;
        return x;
    }

    template <typename T>
    std::vector<T> args_as(std::string flag) {
        flags_.emplace_back(flag, false, "");

        auto parts = detail::split(arg(flag), ",");
        std::vector<T> xs;
        for (auto part : parts) {
            auto value = std::stringstream(part);
            T x = {};
            value >> x;
            xs.push_back(x);
        }

        return xs;
    }

    template <typename T>
    T arg_as_or(std::string flag, T alt) {
        flags_.emplace_back(flag, true, std::to_string(alt));

        if (!passed(flag)) {
            return alt;
        }
        auto value = std::stringstream(arg(flag));
        T x = {};
        value >> x;
        return x;
    }

    std::string arg_or(std::string flag, std::string alt) {
        flags_.emplace_back(flag, true, alt);

        if (!passed(flag)) {
            return alt;
        }
        return arg(flag);
    }

    bool required_arguments(const std::vector<std::string>& args) {
        for (auto& arg : args) {
            if (!passed(arg)) {
                return false;
            }
        }
        return true;
    }

    bool sane() {
        for (auto [flag, required, alt] : flags_) {
            (void)alt;
            if (required && !passed(flag)) {
                return false;
            }
        }
        return true;
    }

    std::string usage() {
        auto output = std::stringstream("");

        output << "Usage: " << argv[0] << " [OPTIONS]...\n\n";

        for (auto [flag, required, alt] : flags_) {
            output << " " << flag;
            if (required) {
                output << " (" << alt << ")";
            }
            output << "\n";
        }
        return output.str();
    }
};

} // namespace tomo
