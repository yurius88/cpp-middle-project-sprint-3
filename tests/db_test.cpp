#include "book.hpp"
#include "book_database.hpp"
#include "comparators.hpp"
#include "statsistics.hpp"
#include <gtest/gtest.h>

using namespace bookdb;

TEST(TestComponentName, BookDataBaseEmpty) {
    BookDatabase<std::vector<Book>> db;
    EXPECT_EQ(db.empty(), true);
}

TEST(TestComponentName, BookDataBaseNotEmpty) {
    BookDatabase<std::vector<Book>> db;
    db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4., 190);
    EXPECT_EQ(db.empty(), false);
}

TEST(TestComponentName, BookDataBaseCheckSize) {
    BookDatabase<std::vector<Book>> db;
    db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4., 190);
    db.EmplaceBack("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143);
    db.EmplaceBack("The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120);
    EXPECT_EQ(db.size(), 3);
}

TEST(TestComponentName, BookDataBaseCheckAuthorsSize) {
    BookDatabase<std::vector<Book>> db;
    db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4., 190);
    db.EmplaceBack("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143);
    db.EmplaceBack("The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120);
    db.EmplaceBack("The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120);
    EXPECT_EQ(db.GetAuthors().size(), 2);
}

TEST(TestComponentName, BookDataBaseCheckAuthor) {
    BookDatabase<std::vector<Book>> db;
    db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4., 190);
    db.EmplaceBack("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143);
    db.EmplaceBack("The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120);
    ASSERT_TRUE((*db.begin()).author == "George Orwell");
}

TEST(TestComponentName, BookDataBaseCheckSortByAuthor) {
    BookDatabase<std::vector<Book>> db;
    db.EmplaceBack("The Great Gatsby", "Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120);
    db.EmplaceBack("Lord of the Flies", "William Golding", 1954, Genre::Fiction, 4.2, 89);
    db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4., 190);
    std::sort(db.begin(), db.end(), comp::LessByAuthor{});

    ASSERT_TRUE((*db.begin()).author == "George Orwell");
}

TEST(TestComponentName, BookDataBaseCheckSortByPopularity) {
    BookDatabase<std::vector<Book>> db;
    db.EmplaceBack("The Great Gatsby", "Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120);
    db.EmplaceBack("Lord of the Flies", "William Golding", 1954, Genre::Fiction, 4.2, 89);
    db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4., 190);
    std::sort(db.begin(), db.end(), comp::LessByPopularity{});

    ASSERT_TRUE((*db.begin()).title == "Lord of the Flies");
}

TEST(TestComponentName, BookDataBaseCheckHistogram) {
    BookDatabase<std::vector<Book>> db;
    db.EmplaceBack("The Great Gatsby", "Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120);
    db.EmplaceBack("Lord of the Flies", "William Golding", 1954, Genre::Fiction, 4.2, 89);
    db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4., 190);
    db.EmplaceBack("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143);
    auto histogram = buildAuthorHistogramFlat(db);
    ASSERT_TRUE(histogram[*db.GetAuthors().find("George Orwell")] == 2);
}

TEST(TestComponentName, BookDataBaseChecAvgRating) {
    BookDatabase<std::vector<Book>> db;
    db.EmplaceBack("1984", "George Orwell", 1949, Genre::SciFi, 4., 190);
    db.EmplaceBack("Animal Farm", "George Orwell", 1945, Genre::Fiction, 4.4, 143);
    db.EmplaceBack("The Great Gatsby", "F. Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120);
    db.EmplaceBack("To Kill a Mockingbird", "Harper Lee", 1960, Genre::Fiction, 4.8, 156);
    db.EmplaceBack("Pride and Prejudice", "Jane Austen", 1813, Genre::Fiction, 4.7, 178);
    db.EmplaceBack("The Catcher in the Rye", "J.D. Salinger", 1951, Genre::Fiction, 4.3, 112);
    db.EmplaceBack("Brave New World", "Aldous Huxley", 1932, Genre::SciFi, 4.5, 98);
    db.EmplaceBack("Jane Eyre", "Charlotte Brontë", 1847, Genre::Fiction, 4.6, 110);
    db.EmplaceBack("The Hobbit", "J.R.R. Tolkien", 1937, Genre::Fiction, 4.9, 203);
    db.EmplaceBack("Lord of the Flies", "William Golding", 1954, Genre::Fiction, 4.2, 89);
    auto avrRating = calculateAverageRating(db);

    EXPECT_NEAR(avrRating, 4.49f, 1e-3);
}

TEST(TestComponentName, BookDataBaseCheckPushBack) {
    BookDatabase<std::vector<Book>> db;
    Book book1("The Great Gatsby", "Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120);
    db.PushBack(book1);
    Book book2("The Great Gatsby", "Scott Fitzgerald", 1925, Genre::Fiction, 4.5, 120);
    db.PushBack(std::move(book2));
    ASSERT_TRUE(db.size() == 2);
}

TEST(TestComponentName, BookDataBaseCheckAvgRatingEmptyDb) {
    BookDatabase<std::vector<Book>> db;
    auto rating = calculateAverageRating(db);
    EXPECT_NEAR(rating, 0, 1e-1);
}

TEST(TestComponentName, BookDataBaseCheckGenreRatingEmptyDb) {
    BookDatabase<std::vector<Book>> db;
    auto rating = calculateGenreRatings(db.begin(), db.end());
    ASSERT_TRUE(rating.empty());
}
