#include "Format5.h"
#include <stdexcept>
#include <cstring>

static inline int b64val(char c) {
    if (c >= 'A' && c <= 'Z') return c - 'A';
    if (c >= 'a' && c <= 'z') return c - 'a' + 26;
    if (c >= '0' && c <= '9') return c - '0' + 52;
    if (c == '+') return 62;
    if (c == '/') return 63;
    return -1;
}

std::vector<uint8_t> Format5::base64ToBytes(const std::string& b64) {
    std::vector<uint8_t> out;
    int val = 0, valb = -8;
    for (char c : b64) {
        if (isspace((unsigned char)c)) continue;
        if (c == '=') break;
        int d = b64val(c);
        if (d < 0) continue;
        val = (val << 6) + d;
        valb += 6;
        if (valb >= 0) {
            out.push_back((uint8_t)((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
}

void Format5::decodeInto(const std::vector<uint8_t>& input, std::vector<uint8_t>& output) {
    size_t ip = 0;
    size_t outPos = 0;
    const size_t inputLen = input.size();
    const size_t outputLen = output.size();
    while (outPos < outputLen && ip + 4 <= inputLen) {
        uint16_t compressedSize = input[ip] | (input[ip+1] << 8);
        ip += 2;
        uint16_t decompressedSize = input[ip] | (input[ip+1] << 8);
        ip += 2;
        if (compressedSize == 0 || decompressedSize == 0) break;
        if (ip + compressedSize > inputLen) throw std::runtime_error("Format5: truncated input block");
        // If no compression (simple case), compressedSize == decompressedSize -> just copy
        if (compressedSize == decompressedSize) {
            if (outPos + decompressedSize > outputLen) throw std::runtime_error("Format5: output overflow");
            std::memcpy(output.data() + outPos, input.data() + ip, decompressedSize);
            outPos += decompressedSize;
        } else {
            // We don't implement MiniLZO here. Throw to indicate unsupported block.
            throw std::runtime_error("Format5: compressed blocks not supported in this build");
        }
        ip += compressedSize;
    }
}
