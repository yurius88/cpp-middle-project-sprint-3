#pragma once

#include "book.hpp"

namespace bookdb::comp {

struct LessByAuthor {
    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const { return lhs.author < rhs.author; }
};
struct LessByTitle {
    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const { return lhs.title < rhs.title; }
};
struct LessByPopularity {
    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const { return lhs.read_count < rhs.read_count; }
};

struct LessByRating {
    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const { return lhs.rating < rhs.rating; }
};

struct GreaterByRating {
    bool operator()(const bookdb::Book &lhs, const bookdb::Book &rhs) const { return lhs.rating > rhs.rating; }
};

}  // namespace bookdb::comp