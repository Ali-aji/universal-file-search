#pragma once

#include "ufs/core/IParser.hpp"

namespace ufs::parsers {

/**
 * JsonParser handles JSON Lines format (.jsonl, or .json where each
 * line is a standalone JSON object). Flattens top-level keys into
 * Record fields. Nested objects are stringified.
 */
class JsonParser : public core::IParser {
public:
    [[nodiscard]] bool canParse(const std::filesystem::path& file) const override;
    [[nodiscard]] std::vector<core::Record> parse(const std::filesystem::path& file) const override;
    [[nodiscard]] std::string name() const override { return "JsonParser"; }
};

} // namespace ufs::parsers