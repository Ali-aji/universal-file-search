#pragma once

#include "ufs/core/IParser.hpp"
#include <memory>
#include <vector>
#include <filesystem>
#include <stdexcept>

namespace ufs::core {

class NoParserFoundError : public std::runtime_error {
public:
    explicit NoParserFoundError(const std::filesystem::path& file)
        : std::runtime_error("No parser found for file: " + file.string()) {}
};

/**
 * ParserRegistry holds all registered parsers and resolves which one
 * should handle a given file.
 *
 * Adding a new format = write a parser class + register one instance
 * here. Nothing else in the codebase changes.
 */
class ParserRegistry {
public:
    void registerParser(std::shared_ptr<IParser> parser);

    // Returns the first parser that claims it can handle this file.
    // Throws NoParserFoundError if none can.
    [[nodiscard]] std::shared_ptr<IParser> resolve(const std::filesystem::path& file) const;

    [[nodiscard]] const std::vector<std::shared_ptr<IParser>>& all() const {
        return parsers_;
    }

private:
    std::vector<std::shared_ptr<IParser>> parsers_;
};

} // namespace ufs::core