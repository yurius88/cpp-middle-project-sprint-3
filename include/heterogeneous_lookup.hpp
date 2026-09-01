#pragma once

#include <string>
#include <string_view>

namespace bookdb {

struct TransparentStringLess {
    using is_transparent = void;
    bool operator()(std::string_view a, std::string_view b) const noexcept { return a < b; }
    bool operator()(const std::string &a, std::string_view b) const noexcept { return std::string_view(a) < b; }
    bool operator()(std::string_view a, const std::string &b) const noexcept { return a < std::string_view(b); }
    bool operator()(const std::string &lhs, const std::string &rhs) const noexcept { return lhs < rhs; };
};

struct TransparentStringEqual {
    using is_transparent = void;
    bool operator()(std::string_view a, std::string_view b) const noexcept { return a == b; }
    bool operator()(const std::string &a, std::string_view b) const noexcept { return std::string_view(a) == b; }
    bool operator()(std::string_view a, const std::string &b) const noexcept { return a == std::string_view(b); }
    bool operator()(const std::string &lhs, const std::string &rhs) const noexcept { return lhs == rhs; };
};

struct TransparentStringHash {
    using is_transparent = void;
    size_t operator()(std::string_view sv) const noexcept { return std::hash<std::string_view>{}(sv); }

    size_t operator()(const std::string &str) const noexcept { return std::hash<std::string_view>{}(str); }
};

}  // namespace bookdb
