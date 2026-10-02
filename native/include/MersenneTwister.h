#pragma once
#include <cstdint>
#include <cmath>

class MersenneTwister {
public:
    explicit MersenneTwister(uint32_t seed = 5489u);

    double random();
    int rangeInt(int min, int max);

private:
    static constexpr int N = 624;
    static constexpr int M = 397;
    static constexpr uint32_t MATRIX_A = 0x9908B0DFu;
    static constexpr uint32_t UPPER_MASK = 0x80000000u;
    static constexpr uint32_t LOWER_MASK = 0x7FFFFFFFu;

    uint32_t mt_[N];
    int index_;

    void generate();
};
