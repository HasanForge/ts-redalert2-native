#include "MersenneTwister.h"
#include <cmath>

MersenneTwister::MersenneTwister(uint32_t seed) : index_(N) {
    mt_[0] = seed & 0xFFFFFFFFu;
    for (int i = 1; i < N; ++i) {
        mt_[i] = (1812433253u * (mt_[i - 1] ^ (mt_[i - 1] >> 30)) + static_cast<uint32_t>(i)) & 0xFFFFFFFFu;
    }
}

void MersenneTwister::generate() {
    for (int i = 0; i < N; ++i) {
        uint32_t y = (mt_[i] & UPPER_MASK) | (mt_[(i + 1) % N] & LOWER_MASK);
        uint32_t xA = mt_[(i + M) % N] ^ (y >> 1);
        if ((y & 1u) != 0u) {
            xA ^= MATRIX_A;
        }
        mt_[i] = xA;
    }
    index_ = 0;
}

double MersenneTwister::random() {
    if (index_ >= N) {
        generate();
    }

    uint32_t y = mt_[index_++];
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9D2C5680u;
    y ^= (y << 15) & 0xEFC60000u;
    y ^= (y >> 18);

    return static_cast<double>(y) / 4294967296.0;
}

int MersenneTwister::rangeInt(int min, int max) {
    const double r = random();
    return static_cast<int>(std::floor(r * static_cast<double>(max - min + 1))) + min;
}
