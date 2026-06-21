#include "ufs/core/ParserRegistry.hpp"
#include "ufs/parsers/LogParser.hpp"
#include "ufs/parsers/CsvParser.hpp"
#include "ufs/parsers/JsonParser.hpp"
#include "ufs/parsers/TextParser.hpp"
#include "ufs/engine/SearchEngine.hpp"

#include <iostream>
#include <string>
#include <vector>

namespace {

void printUsage() {
    std::cerr <<
        "Usage: ufs <command> [options]\n\n" <<
        "Commands:\n" <<
        "  search <keyword> --file <path>   Search for a keyword in a file\n\n" <<
        "Examples:\n" <<
        "  ufs search \"ERROR\" --file app.log\n";
}

ufs::core::ParserRegistry buildRegistry() {
    ufs::core::ParserRegistry registry;
    registry.registerParser(std::make_shared<ufs::parsers::LogParser>());
    registry.registerParser(std::make_shared<ufs::parsers::CsvParser>());
    registry.registerParser(std::make_shared<ufs::parsers::JsonParser>());
    registry.registerParser(std::make_shared<ufs::parsers::TextParser>()); // fallback, must be last
    return registry;
}

int runSearch(const std::vector<std::string>& args) {
    // Expected: <keyword> --file <path>
    if (args.size() < 3 || args[1] != "--file") {
        std::cerr << "Usage: ufs search <keyword> --file <path>\n";
        return 1;
    }

    const std::string& keyword = args[0];
    const std::string& filePath = args[2];

    auto registry = buildRegistry();
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

} // namespace

int main(int argc, char** argv) {
    if (argc < 2) {
        printUsage();
        return 1;
    }

    std::string command = argv[1];
    std::vector<std::string> args(argv + 2, argv + argc);

    if (command == "search") {
        return runSearch(args);
    }

    if (command == "--help" || command == "-h") {
        printUsage();
        return 0;
    }

    std::cerr << "Unknown command: " << command << "\n\n";
    printUsage();
    return 1;
}