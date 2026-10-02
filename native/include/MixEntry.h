#pragma once
#include <cstdint>
#include <string>
#include <vector>

class MixEntry {
public:
    static uint32_t hashFilename(const std::string& filename, bool debug = false);

    uint32_t hash = 0;
    uint32_t offset = 0;
    uint32_t length = 0;
};
