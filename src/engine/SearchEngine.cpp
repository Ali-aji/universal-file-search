// src/engine/SearchEngine.cpp
#include "ufs/engine/SearchEngine.hpp"
#include <algorithm>

namespace ufs::engine {

std::vector<core::Record> SearchEngine::search(
    const std::filesystem::path& file,
    const std::string& keyword) const
{
    auto parser = registry_.resolve(file);
    auto records = parser->parse(file);

    std::vector<core::Record> matches;
    for (auto& rec : records) {
        if (rec.raw.find(keyword) != std::string::npos) {
            matches.push_back(std::move(rec));
        }
    }
    return matches;
}

} // namespace ufs::engine