#include "utils/rng.hpp"

#include <cstdint>
#include <random>
#include <time.h>


static RandomNumberGenerator* _instance = nullptr;

RandomNumberGenerator* RandomNumberGenerator::GetInstance()
{
    if (_instance == nullptr) {
        _instance = new RandomNumberGenerator();
    }
    return _instance;
}

RandomNumberGenerator::RandomNumberGenerator()
{
    std::random_device rd;      // Get a random seed from the OS entropy device, or whatever
    std::mt19937_64 _eng(rd()); // Use the 64-bit Mersenne Twister 19937 generator and seed it with entropy.
    this->eng = _eng;
}

uint64_t RandomNumberGenerator::getRandomNumber()
{
    std::uniform_int_distribution<uint64_t> distr;
    return distr(this->eng);
}

uint64_t RandomNumberGenerator::getRandomNumber(uint64_t min, uint64_t max)
{
    std::uniform_int_distribution<uint64_t> distr;
    return (distr(this->eng) %( max - min)) + min;
}

void RandomNumberGenerator::Cleanup(void) const
{
    if (_instance != nullptr)
        delete _instance;
}