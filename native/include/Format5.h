#pragma once
#include <cstdint>
#include <vector>

namespace Format5 {
    // Decode input into output. Supports a simple case where compressedSize == decompressedSize (no compression).
    // Input format: sequence of blocks: [uint16 compressedSize][uint16 decompressedSize][compressed bytes...]
    void decodeInto(const std::vector<uint8_t>& input, std::vector<uint8_t>& output);
    std::vector<uint8_t> base64ToBytes(const std::string& b64);
}
