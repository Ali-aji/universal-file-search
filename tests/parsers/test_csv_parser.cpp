// tests/parsers/test_csv_parser.cpp
#include <catch2/catch_test_macros.hpp>
#include "ufs/parsers/CsvParser.hpp"
#include <fstream>

TEST_CASE("CsvParser parses headers into fields", "[csv]") {
    // Arrange: write a temp CSV file
    std::filesystem::path tmp = std::filesystem::temp_directory_path() / "test.csv";
    {
        std::ofstream out(tmp);
        out << "name,age,city\n";
        out << "Ali,30,Nurnberg\n";
        out << "Sara,25,Berlin\n";
    }

    ufs::parsers::CsvParser parser;

    SECTION("canParse accepts .csv files") {
        REQUIRE(parser.canParse(tmp) == true);
    }

    SECTION("parse produces correct Records") {
        auto records = parser.parse(tmp);

        REQUIRE(records.size() == 2);

        CHECK(records[0].get("name") == "Ali");
        CHECK(records[0].get("age") == "30");
        CHECK(records[0].get("city") == "Nurnberg");
        CHECK(records[0].lineNumber == 2);

        CHECK(records[1].get("name") == "Sara");
    }

    std::filesystem::remove(tmp);
}