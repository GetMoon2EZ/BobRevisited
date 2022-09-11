#pragma once

#include <cstdint>
#include <iostream>

#include "entities/entity.hpp"
#include "utils/types.hpp"


class Food: public Entity
{
public:
    static const energy_t FOOD_DEFAULT_ENERGY;

    Food();
    Food(position_t x, position_t y, energy_t energy = FOOD_DEFAULT_ENERGY);

    /**
     * @brief Try to consume a given amount of energy. 
     * If there is not enough energy, the maximum amount of energy will be consumed.
     * 
     * @param[in] amount The max amount of energy to consume.
     * @return energy_t The amount of energy that was consumed.
     */
    energy_t Consume(energy_t amount);

    void AddEnergy(energy_t to_add);
    void RemoveEnergy(energy_t to_remove);

    energy_t getEnergy(void) const;
    energy_t getEnergyMax(void) const;

    friend std::ostream& operator<<(std::ostream& out, const Food& food);

    ~Food() = default;

private:
    energy_t energy = FOOD_DEFAULT_ENERGY;
    energy_t energyMax = FOOD_DEFAULT_ENERGY;
};
