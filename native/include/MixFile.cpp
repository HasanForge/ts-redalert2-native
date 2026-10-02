#include "MixFile.h"
#include "MixEntry.h"
#include "DataStream.h"
#include <cstring>
#include <stdexcept>

MixFile::MixFile(const std::vector<uint8_t>& data) : data_(data), dataStart_(0) {
    size_t pos = 0;
    // Try to parse as TD header matching TypeScript parseTdHeader
    parseTdHeader(data_, pos);
    dataStart_ = pos;
}

void MixFile::parseTdHeader(const std::vector<uint8_t>& src, size_t& pos) {
    // Expect at least 6 bytes for count + reserved
    if (src.size() < 6) throw std::runtime_error("MixFile: too small for header");
    DataStream ds(src.data(), src.size());
    uint16_t count = ds.readUint16LE();
    ds.readUint32LE(); // reserved
    size_t entriesStart = ds.position();
    for (uint16_t i = 0; i < count; ++i) {
        if (ds.position() + 12 > ds.byteLength()) break;
        uint32_t h = ds.readUint32LE();
        uint32_t off = ds.readUint32LE();
        uint32_t len = ds.readUint32LE();
        Entry e{h, off, len};
        // only store first occurrence
        if (index_.find(h) == index_.end()) index_[h] = e;
    }
    pos = ds.position();
}

bool MixFile::containsFile(const std::string& filename) const {
    uint32_t h = MixEntry::hashFilename(filename, false);
    return index_.find(h) != index_.end();
}

std::vector<uint8_t> MixFile::openFile(const std::string& filename) const {
    uint32_t h = MixEntry::hashFilename(filename, false);
    auto it = index_.find(h);
    if (it == index_.end()) throw std::runtime_error("MixFile: file not found");
    const Entry& e = it->second;
    if (e.offset + e.length > data_.size()) throw std::runtime_error("MixFile: entry out of range");
    std::vector<uint8_t> out(e.length);
    std::memcpy(out.data(), data_.data() + e.offset, e.length);
    return out;
}
