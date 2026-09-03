#pragma once

#include <concepts>
#include <iterator>
#include <type_traits>

#include "book.hpp"

namespace bookdb {

template <typename T>
concept BookContainerLike = requires(T container, Book book) {
    typename T::value_type;

    requires std::same_as<std::remove_cvref_t<typename T::value_type>, Book>;

    { container.size() } -> std::integral;
    { container.empty() } -> std::same_as<bool>;

    container.push_back(book);
    container.push_back(book);
};

template <typename T>
concept BookIterator = std::forward_iterator<T>;

template <typename S, typename I>
concept BookSentinel = std::sentinel_for<S, I>;

template <typename P>
concept BookPredicate = requires(P predicate, const Book &lhs) {
    { predicate(lhs) } -> std::convertible_to<bool>;
};

template <typename C>
concept BookComparator = requires(C comparator, const Book &lhs, const Book &rhs) {
    { comparator(lhs, rhs) } -> std::convertible_to<bool>;
};

}  // namespace bookdb