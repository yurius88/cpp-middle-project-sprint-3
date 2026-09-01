#pragma once

#include <format>
#include <sstream>
#include <stdexcept>
#include <string_view>

namespace bookdb {

enum class Genre { Fiction, NonFiction, SciFi, Biography, Mystery, Unknown };

// Ваш код для constexpr преобразования строк в enum::Genre и наоборот здесь

// 1. Преобразование из строки в Enum
constexpr Genre GenreFromString(std::string_view s) {
    if (s == "Fiction")
        return Genre::Fiction;
    if (s == "NonFiction")
        return Genre::NonFiction;
    if (s == "SciFi")
        return Genre::SciFi;
    if (s == "Biography")
        return Genre::Biography;
    if (s == "Mystery")
        return Genre::Mystery;
    return Genre::Unknown;
}
// 2. Преобразование из Enum в строку
constexpr std::string_view GenreToString(Genre g) {
    switch (g) {
    case Genre::Fiction:
        return "Fiction";
    case Genre::NonFiction:
        return "NonFiction";
    case Genre::SciFi:
        return "SciFi";
    case Genre::Biography:
        return "Biography";
    case Genre::Mystery:
        return "Mystery";
    case Genre::Unknown:
    default:
        return "Unknown";
    }
}

struct Book {
    // string_view для экономии памяти, чтобы ссылаться на оригинальную строку, хранящуюся в другом контейнере
    std::string title;
    std::string_view author;

    int year;
    Genre genre;
    double rating;
    int read_count;

    // Ваш код для конструкторов здесь
    constexpr Book(const std::string &_title, std::string_view _author, int _year, std::string_view genreStr,
                   double _rating, int _read_count)
        : title(_title), author(_author), year(_year), genre(GenreFromString(genreStr)), rating(_rating),
          read_count(_read_count) {}
    constexpr Book(const std::string &_title, std::string_view _author, int _year, Genre _genre, double _rating,
                   int _read_count)
        : title(_title), author(_author), year(_year), genre(_genre), rating(_rating), read_count(_read_count) {}
};
}  // namespace bookdb

namespace std {
template <>
struct formatter<bookdb::Genre, char> {
    template <typename FormatContext>
    auto format(const bookdb::Genre g, FormatContext &fc) const {
        std::string genre_str;

        // clang-format off
        using bookdb::Genre;
        switch (g) {
            case Genre::Fiction:    genre_str = "Fiction"; break;
            case Genre::Mystery:    genre_str = "Mystery"; break;
            case Genre::NonFiction: genre_str = "NonFiction"; break;
            case Genre::SciFi:      genre_str = "SciFi"; break;
            case Genre::Biography:  genre_str = "Biography"; break;
            case Genre::Unknown:    genre_str = "Unknown"; break;
            default:
                throw logic_error{"Unsupported bookdb::Genre"};
            }
        // clang-format on
        return format_to(fc.out(), "{}", genre_str);
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

// Ваш код для std::formatter<Book> здесь
template <>
struct formatter<bookdb::Book, char> {

    std::formatter<bookdb::Genre> genre_formatter;

    template <typename FormatContext>
    auto format(const bookdb::Book b, FormatContext &fc) const {
        // clang-format on
        format_to(fc.out(), "Author: {}\nTitle {}\nYear {}\nGenre ", b.author, b.title, b.year);
        genre_formatter.format(b.genre, fc);
        return format_to(fc.out(), "\nRating {}\nRead count {}\n", b.rating, b.read_count);
    }

    constexpr auto parse(format_parse_context &ctx) {
        return ctx.begin();  // Просто игнорируем пользовательский формат
    }
};

}  // namespace std
