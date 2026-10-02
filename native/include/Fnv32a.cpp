#include "Fnv32a.h"

uint32_t fnv1a_32(const uint8_t* data, size_t len) {
    // Exact TypeScript behavior from src/util/math.ts:
    // hash ^= data[i];
    // hash += (hash << 1) + (hash << 4) + (hash << 7) + (hash << 8) + (hash << 24);
    uint32_t hash = 0x811C9DC5u;
    for (size_t i = 0; i < len; ++i) {
        hash ^= data[i];
        hash = (hash + (hash << 1) + (hash << 4) + (hash << 7) + (hash << 8) + (hash << 24)) & 0xFFFFFFFFu;
    }
    return hash;
}
