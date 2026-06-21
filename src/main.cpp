#include "ufs/core/ParserRegistry.hpp"
#include "ufs/parsers/LogParser.hpp"
#include "ufs/parsers/TextParser.hpp"
#include "ufs/engine/SearchEngine.hpp"
#include "ufs/parsers/CsvParser.hpp"
#include "ufs/parsers/JsonParser.hpp"

#include <iostream>
#include <string>

void printUsage(const char* prog) {
    std::cerr << "Usage: " << prog << " <keyword> --file <path>\n";
}

int main(int argc, char** argv) {
    if (argc < 4) {
        printUsage(argv[0]);
        return 1;
    }

    std::string keyword = argv[1];
    std::string flag = argv[2];
    std::string filePath = argv[3];

    if (flag != "--file") {
        printUsage(argv[0]);
        return 1;
    }

    // Register parsers. Order matters: specific parsers first,
    // TextParser last as the fallback.
    ufs::core::ParserRegistry registry;
    registry.registerParser(std::make_shared<ufs::parsers::LogParser>());
    registry.registerParser(std::make_shared<ufs::parsers::CsvParser>());
    registry.registerParser(std::make_shared<ufs::parsers::JsonParser>());
    registry.registerParser(std::make_shared<ufs::parsers::TextParser>());

    ufs::engine::SearchEngine engine(registry);

    try {
        auto results = engine.search(filePath, keyword);

        if (results.empty()) {
            std::cout << "No matches found.\n";
            return 0;
        }

        for (const auto& rec : results) {
            std::cout << "[" << rec.sourceFile.filename().string()
                       << ":" << rec.lineNumber << "] "
                       << rec.raw << "\n";
        }
        std::cout << "\n" << results.size() << " match(es) found.\n";

    } catch (const std::exception& ex) {
        std::cerr << "Error: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}