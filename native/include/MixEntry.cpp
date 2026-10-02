#include "MixEntry.h"
#include "Crc32.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <vector>

static std::string toUpperAscii(const std::string& s) {
    std::string out = s;
    std::transform(out.begin(), out.end(), out.begin(), [](unsigned char ch) {
        return static_cast<char>(std::toupper(ch));
    });
    return out;
}

uint32_t MixEntry::hashFilename(const std::string& filename, bool debug) {
    std::string processed = toUpperAscii(filename);
    const size_t originalLength = processed.size();
    const size_t r = originalLength >> 2;

    if (debug) {
        std::cout << "[hashFilename] Original: \"" << filename
                  << "\", Uppercased: \"" << processed
                  << "\", Length: " << originalLength << "\n";
    }

    if ((originalLength & 3u) != 0u) {
        const int appendCharCode = static_cast<int>(originalLength - (r << 2));
        processed.push_back(static_cast<char>(appendCharCode));
        if (debug) {
            std::cout << "[hashFilename] Appended char code: " << appendCharCode
                      << ", Name after append: \"" << processed << "\"\n";
        }

        const int numPaddingChars = 3 - static_cast<int>(originalLength & 3u);
        const size_t paddingCharSourceIndex = r << 2;
        unsigned char charToPadCode = 0;
        if (paddingCharSourceIndex < processed.size()) {
            charToPadCode = static_cast<unsigned char>(processed[paddingCharSourceIndex]);
        }
        for (int i = 0; i < numPaddingChars; ++i) {
            processed.push_back(static_cast<char>(charToPadCode));
        }
        if (debug) {
            std::cout << "[hashFilename] Name after padding: \"" << processed
                      << "\", Final Length: " << processed.size() << "\n";
        }
    }

    std::vector<uint8_t> nameBytes(processed.begin(), processed.end());
    const uint32_t crc = Crc32::calculateCrc(nameBytes.data(), nameBytes.size());
    if (debug) {
        std::cout << "[hashFilename] Calculated CRC: " << crc
                  << " (0x" << std::hex << crc << std::dec << ")\n";
    }
    return crc;
}
