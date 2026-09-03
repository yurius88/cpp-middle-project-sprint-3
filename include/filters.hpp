#pragma once

#include <algorithm>
#include <functional>

#include "book.hpp"
#include "concepts.hpp"

namespace bookdb {
constexpr auto YearBetween(int start, int end) {
    return [start, end](const Book &book) noexcept -> bool { return book.year >= start && book.year <= end; };
}
constexpr auto RatingAbove(double min_rating) {
    return [min_rating](const Book &book) noexcept -> bool { return book.rating > min_rating; };
}
constexpr auto GenreIs(Genre genre) {
    return [genre](const Book &book) noexcept -> bool { return book.genre == genre; };
}
}  // namespace bookdb