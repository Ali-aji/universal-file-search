#pragma once

#include "ufs/core/ParserRegistry.hpp"
#include "ufs/core/Record.hpp"
#include <vector>
#include <string>
#include <filesystem>

namespace ufs::engine {

/**
 * SearchEngine is the orchestrator: given a file and a keyword,
 * it resolves the right parser, parses the file into Records, and
 * returns the ones that match.
 *
 * For now "match" means simple substring search on the raw line.
 * This gets replaced by the Evaluator (AST-based predicate matching)
 * in a later phase — SearchEngine's job stays the same either way.
 */
class SearchEngine {
public:
    explicit SearchEngine(const core::ParserRegistry& registry)
        : registry_(registry) {}

    // Returns every Record whose raw text contains `keyword`.
    [[nodiscard]] std::vector<core::Record> search(
        const std::filesystem::path& file,
        const std::string& keyword) const;

private:
    const core::ParserRegistry& registry_;
};

} // namespace ufs::engine