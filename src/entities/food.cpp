#include "entities/food.hpp"

#include <cstdint>

#include "utils/types.hpp"


const energy_t Food::FOOD_DEFAULT_ENERGY = (energy_t) 100;

Food::Food(position_t _x, position_t _y): Entity(_x, _y)
{}

energy_t Food::Consume(energy_t amount)
{
    energy_t consumed = 0;
    if (amount <= this->energy) {
        consumed = amount;
        this->energy -= amount;
    } else {
        consumed = this->energy;
        this->energy = 0;
    }
    return consumed;
}

energy_t Food::getEnergy(void) const
{
    return this->energy;
}
