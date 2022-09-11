#include "utils/entity_utils.hpp"

#include <cstdint>
#include <random>

#include "entities/bob.hpp"
#include "utils/rng.hpp"
#include "utils/types.hpp"


void getRandomCoordinates(dimension_t max_x, dimension_t max_y, position_t *x, position_t *y)
{
    RandomNumberGenerator *rng = RandomNumberGenerator::GetInstance();
    position_t random_x = static_cast<position_t>(rng->getRandomNumber());
    position_t random_y = static_cast<position_t>(rng->getRandomNumber());

    *x = random_x % static_cast<position_t>(max_x);
    *y = random_y % static_cast<position_t>(max_y);
}
