// src/parsers/LogParser.cpp
#include "ufs/parsers/LogParser.hpp"
#include <fstream>
#include <regex>
#include <stdexcept>

namespace ufs::parsers {

bool LogParser::canParse(const std::filesystem::path& file) const {
    return file.extension() == ".log";
}

std::vector<core::Record> LogParser::parse(const std::filesystem::path& file) const {
    std::ifstream in(file);
    if (!in) {
        throw std::runtime_error("LogParser: cannot open file: " + file.string());
    }

    // Matches: "2026-06-20 14:32:11 ERROR message..."
    // or:      "2026-06-20 14:32:11 [ERROR] message..."
    static const std::regex pattern(
        R"((\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2})\s+\[?(\w+)\]?\s+(.*))"
    );

    std::vector<core::Record> records;
    std::string line;
    std::size_t lineNo = 0;

    while (std::getline(in, line)) {
        ++lineNo;
        if (line.empty()) continue;

        core::Record rec;
        rec.raw = line;
        rec.lineNumber = lineNo;
        rec.sourceFile = file;

        std::smatch match;
        if (std::regex_match(line, match, pattern)) {
            rec.fields["timestamp"] = match[1].str();
            rec.fields["level"]     = match[2].str();
            rec.fields["message"]   = match[3].str();
        } else {
            // Doesn't match the expected format — still keep it
            // searchable via raw text, just without structured fields.
            rec.fields["message"] = line;
        }

        records.push_back(std::move(rec));
    }

    return records;
}

} // namespace ufs::parsers