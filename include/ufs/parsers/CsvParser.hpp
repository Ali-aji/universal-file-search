#pragma once

#include "ufs/core/IParser.hpp"

namespace ufs::parsers {

/**
 * CsvParser reads the first line as headers, then converts every
 * subsequent line into a Record where fields map header -> value.
 *
 * Does NOT handle quoted commas (e.g. "value, with comma") in v1 —
 * that's a known limitation documented in parsers.md, addressed
 * if needed later.
 */
class CsvParser : public core::IParser {
public:
    [[nodiscard]] bool canParse(const std::filesystem::path& file) const override;
    [[nodiscard]] std::vector<core::Record> parse(const std::filesystem::path& file) const override;
    [[nodiscard]] std::string name() const override { return "CsvParser"; }
};

} // namespace ufs::parsers