#pragma once
#include <cstddef>
#include <cstdint>

class Crc32 {
public:
    Crc32(uint32_t initial = 0xFFFFFFFFu);

    void append(const uint8_t* data, size_t len);
    uint32_t get() const;
    static uint32_t calculateCrc(const uint8_t* data, size_t len, uint32_t initial = 0xFFFFFFFFu);

private:
    uint32_t crc_;
    uint32_t initialCrc_;
    static const uint32_t TABLE[256];
};
