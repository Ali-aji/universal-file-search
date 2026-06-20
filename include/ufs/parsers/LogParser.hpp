#pragma once

#include "ufs/core/IParser.hpp"

namespace ufs::parsers {

/**
 * LogParser handles common log line formats:
 *   2026-06-20 14:32:11 ERROR connection timeout
 *   2026-06-20 14:32:11 [ERROR] connection timeout
 *
 * Extracts fields: timestamp, level, message.
 * Claims files with .log extension.
 */
class LogParser : public core::IParser {
public:
    [[nodiscard]] bool canParse(const std::filesystem::path& file) const override;
    [[nodiscard]] std::vector<core::Record> parse(const std::filesystem::path& file) const override;
    [[nodiscard]] std::string name() const override { return "LogParser"; }
};

} // namespace ufs::parsers