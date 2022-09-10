#pragma once

#include <cstdint>
#include <random>


class RandomNumberGenerator
{
public:
    static RandomNumberGenerator* GetInstance();

    /**
     * @brief Get a random number from 0 to UINT64_MAX
     * 
     * @return uint64_t The random number
     */
    uint64_t getRandomNumber();

    /**
     * @brief Get a random number from [min;max[
     * 
     * @param[in] min
     * @param[in] max
     * 
     * @return uint64_t The random number
     */
    uint64_t getRandomNumber(uint64_t min, uint64_t max);

    void Cleanup(void) const;

    RandomNumberGenerator(RandomNumberGenerator &other) = delete;
    void operator=(const RandomNumberGenerator &) = delete;

private:
    std::mt19937_64 eng;

    RandomNumberGenerator();
};
