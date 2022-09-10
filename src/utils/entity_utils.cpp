#include "utils/entity_utils.hpp"

#include <cstdint>
#include <random>

#include "utils/rng.hpp"


void getRandomCoordinates(uint32_t max_x, uint32_t max_y, uint32_t *x, uint32_t *y)
{
    RandomNumberGenerator *rng = RandomNumberGenerator::GetInstance();
    uint32_t random_x = static_cast<uint32_t>(rng->getRandomNumber());
    uint32_t random_y = static_cast<uint32_t>(rng->getRandomNumber());

    *x = random_x % max_x;
    *y = random_y % max_y;
}
