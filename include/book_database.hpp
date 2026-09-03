#pragma once

#include <initializer_list>
#include <print>
#include <set>
#include <string>
#include <string_view>
#include <unordered_set>
#include <vector>

#include "book.hpp"
#include "concepts.hpp"
#include "heterogeneous_lookup.hpp"

namespace bookdb {

template <BookContainerLike BookContainer = std::vector<Book>>
class BookDatabase {
public:
    // Type aliases

    // Ваш код здесь
    using value_type = Book;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using reference = value_type &;
    using const_reference = const value_type &;

    using pointer = value_type *;
    using const_pointer = const value_type *;

    using iterator = typename BookContainer::iterator;
    using const_iterator = typename BookContainer::const_iterator;

    using AuthorContainer = std::unordered_set<std::string, TransparentStringHash> /* Ваш код здесь */;

    BookDatabase() = default;
    BookDatabase(std::initializer_list<Book> books) : books_(books.begin(), books.end()) {}

    std::span<const Book> GetBooks() const { return std::span<const Book>{books_.data(), books_.size()}; }
    std::span<Book> GetBooks() { return std::span<Book>{books_.data(), books_.size()}; }
    [[nodiscard]] const AuthorContainer &GetAuthors() const { return authors_; }

    void Clear() {
        books_.clear();
        authors_.clear();
    }

    // Standard container interface methods

    // Ваш код здесь

    iterator begin() { return books_.begin(); }
    iterator end() { return books_.end(); }

    const_iterator begin() const noexcept { return books_.begin(); }
    const_iterator end() const noexcept { return books_.end(); }

    const_iterator cbegin() const noexcept { return books_.cbegin(); }
    const_iterator cend() const noexcept { return books_.cend(); }

    bool empty() const noexcept { return books_.empty(); }
    size_type size() const noexcept { return books_.size(); }
    size_type max_size() const noexcept { return books_.max_size(); }

    void PushBack(const Book &book) { EmplaceBack(book); }
    void PushBack(Book &&book) { EmplaceBack(std::move(book)); }

    template <typename... Args>
        requires std::constructible_from<Book, Args...>
    void EmplaceBack(Args &&...args) {
        auto &inserted_book = books_.emplace_back(std::forward<Args>(args)...);
        auto [author_it, _] = authors_.insert(std::string(inserted_book.author));
        inserted_book.author = std::string_view(*author_it);
    }

private:
    BookContainer books_;
    AuthorContainer authors_;
};

}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::BookDatabase<std::vector<bookdb::Book>>> {
    template <typename FormatContext>
    auto format(const bookdb::BookDatabase<std::vector<bookdb::Book>> &db, FormatContext &fc) const {
        /*
        Раскомментируйте, когда bookdb::BookDatabase поддержит интерфейсы, доступные стандартным контейнерам
        (size/begin/...)
        */
        format_to(fc.out(), "BookDatabase (size = {}): ", db.size());

        format_to(fc.out(), "Books:\n");
        for (const auto &book : db.GetBooks()) {
            format_to(fc.out(), "- {}\n", book);
        }

        format_to(fc.out(), "Authors:\n");
        for (const auto &author : db.GetAuthors()) {
            format_to(fc.out(), "- {}\n", author);
        }

        return fc.out();
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};
}  // namespace std
