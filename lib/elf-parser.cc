#include "elf-parser.h"

#include <cstring>
#include <fstream>
#include <iterator>

// Minimal ELF structure definitions - avoids elf.h dependency
namespace {

constexpr uint8_t kElfMagic0 = 0x7f;
constexpr uint8_t kElfMagic1 = 'E';
constexpr uint8_t kElfMagic2 = 'L';
constexpr uint8_t kElfMagic3 = 'F';

constexpr uint8_t kElfClass32 = 1;
constexpr uint8_t kElfDataLsb = 1;  // Little-endian
constexpr uint16_t kElfTypeExec = 2;
constexpr uint16_t kElfTypeRel = 1;

constexpr uint8_t kSymBindGlobal = 1;
constexpr uint8_t kSymTypeFunc = 2;
constexpr uint8_t kSymTypeObject = 1;

constexpr uint16_t kShtSymtab = 2;
constexpr uint16_t kShtStrtab = 3;
constexpr uint16_t kShtNull = 0;
constexpr uint16_t kShtNobits = 8;  // BSS

#pragma pack(push, 1)

struct Elf32Header {
    uint8_t ident[16];
    uint16_t type;
    uint16_t machine;
    uint32_t version;
    uint32_t entry;
    uint32_t phoff;  // Program header offset
    uint32_t shoff;  // Section header offset
    uint32_t flags;
    uint16_t ehsize;
    uint16_t phentsize;
    uint16_t phnum;
    uint16_t shentsize;
    uint16_t shnum;
    uint16_t shstrndx;  // Section name string table index
};

struct Elf32SectionHeader {
    uint32_t name;  // Index into section name string table
    uint32_t type;
    uint32_t flags;
    uint32_t addr;
    uint32_t offset;
    uint32_t size;
    uint32_t link;
    uint32_t info;
    uint32_t addralign;
    uint32_t entsize;
};

struct Elf32Symbol {
    uint32_t name;  // Index into symbol string table
    uint32_t value;
    uint32_t size;
    uint8_t info;  // Bind (high 4 bits) + type (low 4 bits)
    uint8_t other;
    uint16_t shndx;
};

#pragma pack(pop)

}  // anonymous namespace

namespace rv16 {

// ---------------------------------------------------------------------------
// ElfImage convenience methods
// ---------------------------------------------------------------------------

std::optional<std::span<const uint8_t>> ElfImage::findSection(const std::string &name) const {
    for (const auto &section : sections) {
        if (section.name == name)
            return std::span<const uint8_t>(section.data.data(), section.data.size());
    }
    return std::nullopt;
}

std::optional<uint16_t> ElfImage::findSymbol(const std::string &name) const {
    for (const auto &symbol : symbols) {
        if (symbol.name == name) return symbol.address;
    }
    return std::nullopt;
}

// ---------------------------------------------------------------------------
// ElfParser construction
// ---------------------------------------------------------------------------

ElfParser::ElfParser(std::span<const uint8_t> buffer) : buffer_(buffer) {}

// ---------------------------------------------------------------------------
// Entry points
// ---------------------------------------------------------------------------

std::expected<ElfImage, ElfError> ElfParser::fromFile(const std::string &path) {
    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) return std::unexpected(ElfError::fileNotFound);

    std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(file), {});
    if (file.bad()) return std::unexpected(ElfError::readError);

    return fromBuffer(std::span<const uint8_t>(buffer.data(), buffer.size()));
}

std::expected<ElfImage, ElfError> ElfParser::fromBuffer(std::span<const uint8_t> buffer) {
    ElfParser parser(buffer);
    return parser.parse();
}

// ---------------------------------------------------------------------------
// Internal helpers
// ---------------------------------------------------------------------------

namespace {

template <typename T>
const T *bufferAt(std::span<const uint8_t> buf, size_t offset) {
    if (offset + sizeof(T) > buf.size()) return nullptr;
    return reinterpret_cast<const T *>(buf.data() + offset);
}

const char *strtabEntry(std::span<const uint8_t> buf, size_t tableOffset, size_t index) {
    if (tableOffset + index >= buf.size()) return nullptr;
    return reinterpret_cast<const char *>(buf.data() + tableOffset + index);
}

}  // anonymous namespace

// ---------------------------------------------------------------------------
// Parse
// ---------------------------------------------------------------------------

std::expected<ElfImage, ElfError> ElfParser::parse() {
    auto result = validateHeader();
    if (!result) return std::unexpected(result.error());

    ElfImage image;

    auto sectionsResult = loadSections(image);
    if (!sectionsResult) return std::unexpected(sectionsResult.error());

    auto symbolsResult = loadSymbols(image);
    if (!symbolsResult) return std::unexpected(symbolsResult.error());

    const auto *header = bufferAt<Elf32Header>(buffer_, 0);
    image.entryPoint = static_cast<uint16_t>(header->entry);

    return image;
}

// ---------------------------------------------------------------------------
// Header validation
// ---------------------------------------------------------------------------

std::expected<void, ElfError> ElfParser::validateHeader() {
    const auto *header = bufferAt<Elf32Header>(buffer_, 0);
    if (!header) return std::unexpected(ElfError::malformedHeader);

    if (header->ident[0] != kElfMagic0 || header->ident[1] != kElfMagic1 ||
        header->ident[2] != kElfMagic2 || header->ident[3] != kElfMagic3)
        return std::unexpected(ElfError::invalidMagic);

    if (header->ident[4] != kElfClass32) return std::unexpected(ElfError::unsupportedClass);

    if (header->ident[5] != kElfDataLsb) return std::unexpected(ElfError::unsupportedArch);

    return {};
}

// ---------------------------------------------------------------------------
// Section loading
// ---------------------------------------------------------------------------

std::expected<void, ElfError> ElfParser::loadSections(ElfImage &image) {
    const auto *header = bufferAt<Elf32Header>(buffer_, 0);
    if (!header) return std::unexpected(ElfError::malformedHeader);

    // Locate the section name string table
    const auto *shstrHeader = bufferAt<Elf32SectionHeader>(
        buffer_, header->shoff + header->shstrndx * sizeof(Elf32SectionHeader));
    if (!shstrHeader) return std::unexpected(ElfError::malformedHeader);

    size_t shstrOffset = shstrHeader->offset;

    for (uint16_t i = 0; i < header->shnum; ++i) {
        const auto *sh =
            bufferAt<Elf32SectionHeader>(buffer_, header->shoff + i * sizeof(Elf32SectionHeader));
        if (!sh) return std::unexpected(ElfError::malformedHeader);

        // Skip null and symbol/string table sections
        if (sh->type == kShtNull || sh->type == kShtSymtab || sh->type == kShtStrtab) continue;

        const char *namePtr = strtabEntry(buffer_, shstrOffset, sh->name);
        if (!namePtr) return std::unexpected(ElfError::malformedHeader);

        Section section;
        section.name = namePtr;
        section.address = static_cast<uint16_t>(sh->addr);
        section.size = static_cast<uint16_t>(sh->size);

        if (sh->type == kShtNobits) {
            // BSS - no file data, zero-fill
            section.data.assign(sh->size, 0);
        } else {
            if (sh->offset + sh->size > buffer_.size())
                return std::unexpected(ElfError::malformedHeader);
            section.data.assign(buffer_.data() + sh->offset,
                                buffer_.data() + sh->offset + sh->size);
        }

        image.sections.push_back(std::move(section));
    }

    return {};
}

// ---------------------------------------------------------------------------
// Symbol loading
// ---------------------------------------------------------------------------

std::expected<void, ElfError> ElfParser::loadSymbols(ElfImage &image) {
    const auto *header = bufferAt<Elf32Header>(buffer_, 0);
    if (!header) return std::unexpected(ElfError::malformedHeader);

    const Elf32SectionHeader *symtabSh = nullptr;
    const Elf32SectionHeader *strtabSh = nullptr;

    for (uint16_t i = 0; i < header->shnum; ++i) {
        const auto *sh =
            bufferAt<Elf32SectionHeader>(buffer_, header->shoff + i * sizeof(Elf32SectionHeader));
        if (!sh) return std::unexpected(ElfError::malformedHeader);

        if (sh->type == kShtSymtab) symtabSh = sh;
        if (sh->type == kShtStrtab && i != header->shstrndx) strtabSh = sh;
    }

    // No symbol table is not an error - just skip
    if (!symtabSh || !strtabSh) return {};

    size_t numSymbols = symtabSh->size / sizeof(Elf32Symbol);

    for (size_t i = 0; i < numSymbols; ++i) {
        const auto *sym =
            bufferAt<Elf32Symbol>(buffer_, symtabSh->offset + i * sizeof(Elf32Symbol));
        if (!sym) return std::unexpected(ElfError::malformedHeader);

        // Filter to named function and object symbols only
        uint8_t bind = sym->info >> 4;
        uint8_t type = sym->info & 0xf;
        if (sym->name == 0) continue;
        if (type != kSymTypeFunc && type != kSymTypeObject) continue;

        const char *namePtr = strtabEntry(buffer_, strtabSh->offset, sym->name);
        if (!namePtr) continue;

        Symbol symbol;
        symbol.name = namePtr;
        symbol.address = static_cast<uint16_t>(sym->value);
        symbol.size = static_cast<uint16_t>(sym->size);

        image.symbols.push_back(std::move(symbol));
    }

    return {};
}

}  // namespace rv16
