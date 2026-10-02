#pragma once
#include <cstdint>
#include <string>
#include "MersenneTwister.h"

class Prng {
public:
    explicit Prng(uint32_t seed);

    static Prng factory_from_string(const std::string& seed, int sequence);
    double generateRandom();
    int generateRandomInt(int min, int max);
    double getLastRandom() const;

private:
    MersenneTwister mt_;
    double last_;
};
