// rng.hpp - seeded RNG. Seed = group LEADER's student ID -> digits.
#pragma once
#include <random>
#include <cstdint>

inline std::mt19937_64& global_rng() {
    static std::mt19937_64 rng;
    return rng;
}

inline void seed_rng(uint64_t seed) {
    global_rng().seed(seed);
}
