#include "entities/bob.hpp"

#include <iostream>
#include <cstdint>
#include <limits.h>

#include "utils/rng.hpp"
#include "entities/food.hpp"
#include "utils/types.hpp"


const energy_t Bob::BOB_ENERGY_DEFAULT = (energy_t) 100;
const energy_t Bob::BOB_ENERGY_MAX = (energy_t) 200;
const energy_t Bob::REPRODUCTION_COST = (energy_t) 100;
const energy_t Bob::NEW_BORN_ENERGY = (energy_t) 100;

Bob::Bob(position_t _x, position_t _y): Entity(_x, _y)
{}

Bob::Bob(position_t _x, position_t _y, energy_t _energy): Entity(_x, _y)
{
    this->energyLevel = _energy;
}

void Bob::Move(dimension_t world_width, dimension_t world_height)
{
    switch (this->moveHeuristic) {
        case MOVE_HEURISTIC_RANDOM:
            this->MoveRandomly(world_width, world_height);
            break;
    }    
}

void Bob::Eat(Food &food)
{
    energy_t max_can_eat = this->energyMax - this->energyLevel;
    energy_t food_energy = food.getEnergy();
    this->energyLevel += food.Consume(std::min(max_can_eat, food_energy));
    // if (max_can_eat > food_energy) {
    //     this->energyLevel += food.Consume(food_energy);
    // } else {
    //     this->energyLevel = this->energyMax;
    //     food.Consume(max_can_eat);
    // }
}

void Bob::Reproduce()
{
    this->energyLevel -= Bob::REPRODUCTION_COST;
}

void Bob::MoveRandomly(dimension_t world_width, dimension_t world_height)
{
    // Pick a random direction
    RandomNumberGenerator *rng = RandomNumberGenerator::GetInstance();
    MOVE_DIRECTION direction = static_cast<MOVE_DIRECTION>(rng->getRandomNumber(0, static_cast<uint64_t>(MOVE_DIRECTION_COUNT)));

    position_t new_x = this->x, new_y = this->y;
    switch (direction) {
        case UP:
            if (new_y != 0)
                new_y -= 1;
            break;
        case DOWN:
            if (new_y != std::numeric_limits<position_t>::max())
                new_y += 1;
            break;
        case LEFT:
            if (new_x != 0)
                new_x -= 1;
            break;
        case RIGHT:
            if (new_x != std::numeric_limits<position_t>::max())
                new_x += 1;
            break;
        default:
            std::cout << "[ERROR] Unknown direction " << static_cast<int>(direction) << std::endl;
    }

    // Update current energy
    energy_t energy_cons = this->CalculateEnergyConsumption(new_x, new_y);
    // std::cout << "[DEBUG] Energy consumption: " << static_cast<int>(energy_cons) << std::endl;
    if (energy_cons >= this->energyLevel) {
        this->energyLevel = 0;
    } else {
        this->energyLevel -= energy_cons;
    }

    // Check map bounds 
    if (new_x < static_cast<position_t>(world_width) && new_y < static_cast<position_t>(world_height)) {
        this->x = new_x;
        this->y = new_y;
    }

}

energy_t Bob::CalculateEnergyConsumption(position_t new_x, position_t new_y) const
{
    energy_t energy_consumption = 0;

    // Consume 0.5 energy if not moving
    if (this->x == new_x && this->y == new_y)
        return (energy_t) 0.5;

    if (this->y > new_y) {
        energy_consumption += (energy_t) (this->y - new_y);
    } else {
        energy_consumption += (energy_t) (new_y - this->y);
    }

    if (this->x > new_x) {
        energy_consumption += (energy_t) (this->x - new_x);
    } else {
        energy_consumption += (energy_t) (new_x - this->x);
    }

    return energy_consumption;
}

std::ostream& operator<<(std::ostream& out, const Bob& bob)
{
    out << "bob.id = " << static_cast<int64_t>(bob.getID());
    out << "\tbob.x = " << static_cast<int>(bob.getX());
    out << "\tbob.y = " << static_cast<int>(bob.getY());
    out << "\tbob.energyLevel = " << static_cast<int>(bob.getEnergyLevel());
    return out;
}

/*****************************/
/*          Getters          */
/*****************************/

energy_t Bob::getEnergyLevel(void) const
{
    return this->energyLevel;
}

energy_t Bob::getEnergyMax(void) const
{
    return this->energyMax;
}

uint8_t Bob::getHP(void) const
{
    return this->hp;
}

Bob::REPRODUCTION_MODE Bob::getReproductionMode(void) const
{
    return this->reproductionMode;
}
