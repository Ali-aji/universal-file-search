#include "ufs/core/ParserRegistry.hpp"
#include <iostream>

int main() {
    ufs::core::ParserRegistry registry;
    std::cout << "Universal File Search Engine — skeleton builds.\n";
    std::cout << "Registered parsers: " << registry.all().size() << "\n";
    return 0;
}