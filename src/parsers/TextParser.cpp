// src/parsers/TextParser.cpp
#include "ufs/parsers/TextParser.hpp"
#include <fstream>
#include <stdexcept>

namespace ufs::parsers
{

bool TextParser::canParse(const std::filesystem::path&) const
{
    // Fallback parser — always claims it can handle the file.
    // Must be registered last so specific parsers get first refusal.
    return true;
}

std::vector<core::Record> TextParser::parse(const std::filesystem::path& file) const
{
    std::ifstream in(file);
    if (!in)
    {
        throw std::runtime_error("TextParser: cannot open file: " + file.string());
    }

    std::vector<core::Record> records;
    std::string line;
    std::size_t lineNo = 0;

    while (std::getline(in, line))
    {
        ++lineNo;
        core::Record rec;
        rec.raw = line;
        rec.lineNumber = lineNo;
        rec.sourceFile = file;
        rec.fields["line"] = line;
        records.push_back(std::move(rec));
    }

    return records;
}

} // namespace ufs::parsers