#pragma once

#include <cstdint>

#include "entities/entity.hpp"
#include "utils/types.hpp"


class Food: public Entity
{
public:
    static const energy_t FOOD_DEFAULT_ENERGY;

    Food() = default;
    Food(uint32_t x, uint32_t y);

    /**
     * @brief Try to consume a given amount of energy. 
     * If there is not enough energy, the maximum amount of energy will be consumed.
     * 
     * @param[in] amount The max amount of energy to consume.
     * @return energy_t The amount of energy that was consumed.
     */
    energy_t Consume(energy_t amount);

    energy_t getEnergy(void) const;

    ~Food() = default;

private:
    energy_t energy = FOOD_DEFAULT_ENERGY;
};
