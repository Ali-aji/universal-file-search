#pragma once

#include "ufs/core/IParser.hpp"

namespace ufs::parsers
{

/**
 * TextParser treats every line of a file as one Record with a single
 * "line" field plus the raw text. This is the fallback parser — it
 * claims to handle any file, so it must be registered LAST in the
 * ParserRegistry.
 */
class TextParser : public core::IParser
{
  public:
    [[nodiscard]] bool canParse(const std::filesystem::path& file) const override;
    [[nodiscard]] std::vector<core::Record> parse(const std::filesystem::path& file) const override;
    [[nodiscard]] std::string name() const override
    {
        return "TextParser";
    }
};

} // namespace ufs::parsers