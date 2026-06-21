// src/parsers/CsvParser.cpp
#include "ufs/parsers/CsvParser.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

namespace ufs::parsers {

namespace {
std::vector<std::string> splitLine(const std::string& line, char delim = ',') {
    std::vector<std::string> tokens;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, delim)) {
        tokens.push_back(token);
    }
    return tokens;
}
}

bool CsvParser::canParse(const std::filesystem::path& file) const {
    return file.extension() == ".csv";
}

std::vector<core::Record> CsvParser::parse(const std::filesystem::path& file) const {
    std::ifstream in(file);
    if (!in) {
        throw std::runtime_error("CsvParser: cannot open file: " + file.string());
    }

    std::string headerLine;
    if (!std::getline(in, headerLine)) {
        return {}; // empty file
    }
    auto headers = splitLine(headerLine);

    std::vector<core::Record> records;
    std::string line;
    std::size_t lineNo = 1; // header was line 1

    while (std::getline(in, line)) {
        ++lineNo;
        if (line.empty()) continue;

        auto values = splitLine(line);

        core::Record rec;
        rec.raw = line;
        rec.lineNumber = lineNo;
        rec.sourceFile = file;

        for (std::size_t i = 0; i < headers.size() && i < values.size(); ++i) {
            rec.fields[headers[i]] = values[i];
        }

        records.push_back(std::move(rec));
    }

    return records;
}

} // namespace ufs::parsers