// Tests for the CSVRow and CSVField Data Structures

#include "catch.hpp"
#include "csv_parser.hpp"
using namespace csv;

// Construct a CSVRow and assert that its interface works as expected
TEST_CASE("CSVRow Test", "[test_csv_row]") {
    // Create a row of size 4
    auto col_names = std::make_shared<internals::ColNames>(
        std::vector<std::string>({ "A", "B", "C", "D" })
        );

    std::string str = "Col1"
        "Col2"
        "Col3"
        "Col4";

    std::vector<size_t> splits = { 4, 8, 12 };

    CSVRow row(
        std::move(str),
        std::move(splits),
        col_names
    );

    bool error_caught = false;

    SECTION("size() Check") {
        REQUIRE(row.size() == 4);
    }

    SECTION("operator[]") {
        REQUIRE(row[1] == "Col2");
        REQUIRE(row["B"] == "Col2");

        REQUIRE(row[2] == "Col3");
        REQUIRE(row["C"] == "Col3");
    }

    SECTION("operator[] Out of Bounds") {
        try {
            auto dne = row[4].get<>();
        }
        catch (std::runtime_error& err) {
            error_caught = true;
        }

        REQUIRE(error_caught);
    }

    SECTION("operator[] Access Non-Existent Column") {
        try {
            row["Col5"].get<>();
        }
        catch (std::runtime_error& err) {
            error_caught = true;
        }

        REQUIRE(error_caught);
    }

    SECTION("Content Check") {
        REQUIRE(std::vector<std::string>(row) ==
            std::vector<std::string>({ "Col1", "Col2", "Col3", "Col4" }));
    }
}

TEST_CASE("CSVField operator==", "[test_csv_field_equal]") {
    auto col_names = std::make_shared<internals::ColNames>(
        std::vector<std::string>({ "A", "B", "C", "D" })
        );

    std::string str;
    str += "1"
        "2"
        "3"
        "3.14";

    std::vector<size_t> splits = { 1, 2, 3 };
    CSVRow row(std::move(str), std::move(splits), col_names);

    REQUIRE(row["A"] == 1);
    REQUIRE(row["B"] == 2);
    REQUIRE(row["C"] == 3);
    REQUIRE(row["D"].get<double>() == 3.14);
}