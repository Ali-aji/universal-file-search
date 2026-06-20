#pragma once

#include "ufs/core/Record.hpp"
#include <vector>
#include <filesystem>
#include <string>

namespace ufs::core {

/**
 * IParser is the contract every format parser must implement.
 *
 * The SearchEngine never knows whether it's talking to a JSON parser
 * or a log parser — it just calls canParse() to find the right one,
 * then parse() to get Records back.
 */
class IParser {
public:
    virtual ~IParser() = default;

    // Returns true if this parser can handle the given file.
    // Typically checks extension first, falls back to content
    // sniffing (e.g. does the file start with '{' for JSON?).
    [[nodiscard]] virtual bool canParse(const std::filesystem::path& file) const = 0;

    // Parses the file into a sequence of Records.
    // Throws ufs::core::ParseError (defined later) on unrecoverable errors.
    [[nodiscard]] virtual std::vector<Record> parse(const std::filesystem::path& file) const = 0;

    // Human-readable name, used in error messages and --list-parsers output.
    [[nodiscard]] virtual std::string name() const = 0;
};

} // namespace ufs::core