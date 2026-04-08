#include <cstdlib>
#include <format>
#include <iostream>

#include "elf-parser.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << std::format("Usage: {0} <elf-file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const std::string path = argv[1];
    std::cout << std::format("Parsing ELF file: {0}\n", path);

    auto result = rv16::ElfParser::fromFile(path);
    if (!result) {
        switch (result.error()) {
            case rv16::ElfError::fileNotFound:
                std::cerr << std::format("Error: file not found: {0}\n", path);
                break;
            case rv16::ElfError::readError:
                std::cerr << std::format("Error: failed to read file: {0}\n", path);
                break;
            case rv16::ElfError::invalidMagic:
                std::cerr << std::format("Error: not a valid ELF file\n");
                break;
            case rv16::ElfError::unsupportedClass:
                std::cerr << std::format("Error: unsupported ELF class (expected 32-bit)\n");
                break;
            case rv16::ElfError::unsupportedArch:
                std::cerr << std::format("Error: unsupported architecture (expected little-endian)\n");
                break;
            case rv16::ElfError::malformedHeader:
                std::cerr << std::format("Error: malformed ELF header\n");
                break;
        }
        return EXIT_FAILURE;
    }

    const auto &image = *result;

    std::cout << std::format("Entry point : 0x{:04x}\n\n", image.entryPoint);

    std::cout << std::format("Sections ({0}):", image.sections.size());
    for (const auto &section : image.sections) {
        std::cout << std::format("  {:20s}  addr=0x{:04x}  size={:5d} bytes\n", section.name, section.address,
                     section.size);
    }

    if (!image.symbols.empty()) {
        std::cout << std::format("\nSymbols ({}):", image.symbols.size());
        for (const auto &symbol : image.symbols) {
            std::cout << std::format("  {:30s}  addr=0x{:04x}  size={:5d}\n", symbol.name, symbol.address,
                         symbol.size);
        }
    }

    return EXIT_SUCCESS;
}
