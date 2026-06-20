// src/core/ParserRegistry.cpp
#include "ufs/core/ParserRegistry.hpp"

namespace ufs::core {

void ParserRegistry::registerParser(std::shared_ptr<IParser> parser) {
    parsers_.push_back(std::move(parser));
}

std::shared_ptr<IParser> ParserRegistry::resolve(const std::filesystem::path& file) const {
    for (const auto& parser : parsers_) {
        if (parser->canParse(file)) {
            return parser;
        }
    }
    throw NoParserFoundError(file);
}

} // namespace ufs::core