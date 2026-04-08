#pragma once

#include <cstdint>
#include <expected>
#include <optional>
#include <span>
#include <string>
#include <vector>

namespace rv16
{

    struct Section
    {
        std::string name;
        uint16_t address;
        uint16_t size;
        std::vector<uint8_t> data;
    };

    struct Symbol
    {
        std::string name;
        uint16_t address;
        uint16_t size;
    };

    struct ElfImage
    {
        uint16_t entryPoint;
        std::vector<Section> sections;
        std::vector<Symbol> symbols;

        std::optional<std::span<const uint8_t>> findSection(const std::string &name) const;
        std::optional<uint16_t> findSymbol(const std::string &name) const;
    };

    enum class ElfError
    {
        fileNotFound,
        readError,
        invalidMagic,
        unsupportedClass,
        unsupportedArch,
        malformedHeader,
    };

    class ElfParser
    {
    public:
        // Primary entry point - load from file path
        static std::expected<ElfImage, ElfError> fromFile(const std::string &path);

        // Alternate entry point - parse from in-memory buffer (useful for tests)
        static std::expected<ElfImage, ElfError> fromBuffer(std::span<const uint8_t> buffer);

    private:
        explicit ElfParser(std::span<const uint8_t> buffer);

        std::expected<ElfImage, ElfError> parse();

        std::expected<void, ElfError> validateHeader();
        std::expected<void, ElfError> loadSections(ElfImage &image);
        std::expected<void, ElfError> loadSymbols(ElfImage &image);

        std::span<const uint8_t> buffer_;
    };

} // namespace rv16