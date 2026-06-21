// src/parsers/JsonParser.cpp
#include "ufs/parsers/JsonParser.hpp"
#include <nlohmann/json.hpp>
#include <fstream>
#include <stdexcept>

namespace ufs::parsers {

bool JsonParser::canParse(const std::filesystem::path& file) const {
    return file.extension() == ".json" || file.extension() == ".jsonl";
}

std::vector<core::Record> JsonParser::parse(const std::filesystem::path& file) const {
    std::ifstream in(file);
    if (!in) {
        throw std::runtime_error("JsonParser: cannot open file: " + file.string());
    }

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

        try {
            auto j = nlohmann::json::parse(line);
            if (j.is_object()) {
                for (auto& [key, value] : j.items()) {
                    // Flatten: objects/arrays get stringified,
                    // primitives get their plain string form.
                    rec.fields[key] = value.is_string()
                        ? value.get<std::string>()
                        : value.dump();
                }
            }
        } catch (const nlohmann::json::parse_error&) {
            // Not valid JSON on this line — keep it searchable via raw.
            rec.fields["raw"] = line;
        }

        records.push_back(std::move(rec));
    }

    return records;
}

} // namespace ufs::parsers