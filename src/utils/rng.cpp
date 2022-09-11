#include "utils/rng.hpp"

#include <cstdint>
#include <iostream>
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

uint64_t RandomNumberGenerator::getRandomNumber(uint64_t _min, uint64_t _max)
{
    std::uniform_int_distribution<uint64_t> distr;
    return (distr(this->eng) % (_max - _min)) + _min;
}

float RandomNumberGenerator::getRandomFloat(float _min, float _max)
{
    std::uniform_real_distribution<float> distr;
    // std::cout << "[DEBUG] RNG between :" << _min << " and " << _max << std::endl;
    return _min + distr(this->eng) / (distr.max() / (_max - _min));
}

void RandomNumberGenerator::Cleanup(void) const
{
    if (_instance != nullptr)
        delete _instance;
}
