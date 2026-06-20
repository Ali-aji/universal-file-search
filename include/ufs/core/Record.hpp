#pragma once

#include <string>
#include <unordered_map>
#include <filesystem>
#include <cstddef>

namespace ufs::core {

/**
 * Record is the universal representation of a single searchable unit
 * of data, regardless of its source format (log line, CSV row, JSON
 * object, XML element, etc.)
 *
 * Every parser's only responsibility is to produce a vector<Record>.
 * Nothing downstream (query evaluator, merger, formatter) needs to
 * know what format the data originally came from.
 */
struct Record {
    // Key-value fields extracted from the source.
    // e.g. {"level": "ERROR", "message": "timeout", "responseTime": "532"}
    std::unordered_map<std::string, std::string> fields;

    // The original unparsed line/entry, kept for display and fallback
    // text search (e.g. CONTAINS on the whole record).
    std::string raw;

    // 1-based line/row/element number within the source file.
    std::size_t lineNumber = 0;

    // Which file this record came from. Needed once we search
    // across multiple files and merge results.
    std::filesystem::path sourceFile;

    // Convenience accessor. Returns empty string if field doesn't exist.
    [[nodiscard]] const std::string& get(const std::string& key) const {
        static const std::string empty;
        auto it = fields.find(key);
        return it != fields.end() ? it->second : empty;
    }

    [[nodiscard]] bool has(const std::string& key) const {
        return fields.find(key) != fields.end();
    }
};

} // namespace ufs::core