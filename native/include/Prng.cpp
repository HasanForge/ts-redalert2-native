#include "Prng.h"
#include "Crc32.h"
#include <cstdlib>
#include <string>
#include <cmath>

Prng::Prng(uint32_t seed) : mt_(seed), last_(0.0) {}

Prng Prng::factory_from_string(const std::string& seedStr, int sequence) {
    // TypeScript behavior:
    // Number.isNaN(Number(seed)) ? Crc32.calculateCrc(binaryStringToUint8Array(seed as string))
    // : Number(seed + "" + sequence)
    bool isNumeric = !seedStr.empty();
    if (isNumeric) {
        for (char c : seedStr) {
            if (!(c >= '0' && c <= '9')) {
                isNumeric = false;
                break;
            }
        }
    }

    uint32_t numericSeed = 0;
    if (isNumeric) {
        std::string combined = seedStr + std::to_string(sequence);
        unsigned long long value = std::strtoull(combined.c_str(), nullptr, 10);
        numericSeed = static_cast<uint32_t>(value & 0xFFFFFFFFu);
    } else {
        Crc32 crc;
        crc.append(reinterpret_cast<const uint8_t*>(seedStr.data()), seedStr.size());
        numericSeed = crc.get();
    }

    return Prng(numericSeed);
}

double Prng::generateRandom() {
    const double r = mt_.random();
    last_ = r;
    return r;
}

int Prng::generateRandomInt(int min, int max) {
    const double r = mt_.random();
    last_ = r;
    return static_cast<int>(std::floor(r * static_cast<double>(max - min + 1))) + min;
}

double Prng::getLastRandom() const {
    return last_;
}
