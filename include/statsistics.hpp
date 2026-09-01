#pragma once

#include <algorithm>
#include <flat_map>
#include <iterator>
#include <random>
#include <stdexcept>
#include <string_view>

#include "book.hpp"
#include "book_database.hpp"

#include <print>

namespace bookdb {

template <BookContainerLike T, typename Comparator = TransparentStringLess>
auto buildAuthorHistogramFlat(const BookDatabase<T> &cont, Comparator comp = {}) {
    std::flat_map<std::string_view, std::size_t, Comparator> map(comp);
    for (const auto &book : cont.GetBooks()) {
        auto [it, inserted] = map.try_emplace(book.author, 0);
        it->second++;
    }

    return map;
}

template <BookIterator It, BookSentinel<It> Sent>
auto calculateGenreRatings(It first, Sent last) {
    std::flat_map<Genre, std::pair<std::size_t, double>> map;
    std::for_each(first, last, [&](const auto &book) {
        auto [it, inserted] = map.try_emplace(book.genre, std::make_pair(1, book.rating));
        if (!inserted) {
            // увеличиваем кол-во фильмов данного жанра
            it->second.first++;
            it->second.second += book.rating;
        }
    });

    for (auto [key, value] : map) {
        value.second /= value.first;
    }

    return map;
}

template <BookContainerLike T>
double calculateAverageRating(const BookDatabase<T> &books) {
    auto count = std::distance(books.begin(), books.end());
    if (count == 0) {
        return 0.0;
    }

    double sum = std::accumulate(books.begin(), books.end(), 0.0,
                                 [](double current_sum, const Book &book) { return current_sum + book.rating; });

    return sum / count;
}

template <BookContainerLike T>
auto sampleRandomBooks(const BookDatabase<T> &cont, size_t num) {
    std::vector<std::reference_wrapper<const Book>> sampled_books;

    if (num == 0) {
        return sampled_books;
    }

    std::vector<std::reference_wrapper<const Book>> all_books;
    all_books.reserve(cont.size());

    for (auto it = cont.begin(); it != cont.end(); ++it) {
        all_books.push_back(std::cref(*it));
    }

    size_t actual_count = std::min(num, all_books.size());
    sampled_books.reserve(actual_count);

    std::random_device rd;
    std::mt19937 gen(rd());

    std::sample(all_books.begin(), all_books.end(), std::back_inserter(sampled_books), actual_count, gen);

    return sampled_books;
}

template <BookContainerLike T, typename Comparator>
auto getTopNBy(BookDatabase<T> &cont, size_t n, Comparator comp) {
    auto begin_it = cont.begin();
    auto end_it = cont.end();

    size_t total_books = cont.size();

    size_t actual_n = std::min(n, total_books);

    std::partial_sort(begin_it, begin_it + actual_n, end_it, comp);

    std::vector<std::reference_wrapper<const Book>> top_books;
    top_books.reserve(actual_n);

    auto current = begin_it;
    for (size_t i = 0; i < actual_n; ++i, ++current) {
        top_books.push_back(std::cref(*current));
    }

    return top_books;
}

void printHistogram(const auto &histogram) {
    std::println("Author histogram:");
    for (const auto &[author, count] : histogram) {
        std::println("  * {:<15} : {} книг(и)", author, count);
    }
    std::println("---------------------------");
}
void printGenreRatings(const auto &ratings) {
    std::println("Average ratings by genres:");
    for (const auto &[genre, value] : ratings) {
        std::println("  * {:<15} : {}", GenreToString(genre), value.second);
    }
    std::println("---------------------------");
}

constexpr auto YearBetween(int start, int end) {
    return [start, end](const Book &book) noexcept -> bool { return book.year >= start && book.year <= end; };
}
constexpr auto RatingAbove(double min_rating) {
    return [min_rating](const Book &book) noexcept -> bool { return book.rating > min_rating; };
}
constexpr auto GenreIs(Genre genre) {
    return [genre](const Book &book) noexcept -> bool { return book.genre == genre; };
}

template <BookIterator It, BookSentinel<It> Sent, BookPredicate Pred>
auto filterBooks(It first, Sent last, Pred pred) {
    std::vector<std::reference_wrapper<const Book>> container;
    std::copy_if(first, last, std::back_inserter(container), pred);
    return container;
}

template <BookPredicate... Preds>
constexpr auto all_of(Preds... preds) {
    return [preds...](const Book &book) -> bool { return (preds(book) && ...); };
}

template <BookPredicate... Preds>
constexpr auto any_of(Preds... preds) {
    return [preds...](const Book &book) -> bool { return (preds(book) || ...); };
}
}  // namespace bookdb
