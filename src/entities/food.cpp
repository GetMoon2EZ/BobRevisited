#include "entities/food.hpp"

#include <cstdint>
#include <iostream>

#include "utils/types.hpp"


const energy_t Food::FOOD_DEFAULT_ENERGY = (energy_t) 100;

Food::Food(): Entity()
{
    this->energy = 0;
    this->energyMax = 0;
}

Food::Food(position_t _x, position_t _y, energy_t _energy): Entity(_x, _y)
{
    this->energy = _energy;
    this->energyMax = _energy;
}

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

void Food::AddEnergy(energy_t to_add)
{
    this->energyMax += to_add;
    this->energy += to_add;
}

void Food::RemoveEnergy(energy_t to_remove)
{
    if (to_remove > this->energyMax) {
        this->energyMax = 0;
    } else {
        this->energyMax -= to_remove;
    }

    if (to_remove > this->energy) {
        this->energy = 0;
    } else {
        this->energy -= to_remove;
    }
}

std::ostream& operator<<(std::ostream& out, const Food& food)
{
    out << "food.id   = " << food.getID();
    out << "\tfood.pos  = " << static_cast<int>(food.getX()) << ", " << static_cast<int>(food.getY());
    out << "\tfood.energy = " << food.getEnergy();
    out << "\tfood.energyMax = " << food.getEnergyMax();
    return out;
}

energy_t Food::getEnergy(void) const
{
    return this->energy;
}

energy_t Food::getEnergyMax(void) const
{
    return this->energyMax;
}
