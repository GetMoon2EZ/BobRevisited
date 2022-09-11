#include "entities/bob.hpp"

#include <iostream>
#include <cstdint>
#include <limits>
#include <memory>

#include "utils/rng.hpp"
#include "entities/food.hpp"
#include "utils/types.hpp"


const energy_t Bob::BOB_ENERGY_DEFAULT = (energy_t) 100;
const energy_t Bob::BOB_ENERGY_MAX = (energy_t) 200;
const energy_t Bob::REPRODUCTION_COST = (energy_t) 100;
const energy_t Bob::NEW_BORN_ENERGY = (energy_t) 100;
const float Bob::BOB_DEFAULT_VELOCITY = 1.0F;
const mass_t Bob::BOB_DEFAULT_MASS = 1.0F;
const float Bob::CANNIBALISM_MASS_RATIO = 2.0/3.0;

Bob::Bob(position_t _x, position_t _y): Entity(_x, _y)
{}

Bob::Bob(position_t _x, position_t _y, energy_t _energy, float _velocity, float _mass): Entity(_x, _y)
{
    this->energyMax = BOB_ENERGY_MAX;
    this->energyLevel = _energy;
    this->velocity = _velocity;
    this->mass = _mass;
}

float Bob::Move(dimension_t world_width, dimension_t world_height, float move_amount)
{
    if (this->movePercentage != 100) {
        float ret = 0;
        if (move_amount > (this->movePercentage / 100)) {
            ret = this->movePercentage;
            this->movePercentage = 100;
        } else {
            ret = move_amount;
            this->movePercentage -= (move_amount * 100);
        }
        return ret;
    }

    switch (this->moveHeuristic) {
        case MOVE_HEURISTIC_RANDOM:
            return this->MoveRandomly(world_width, world_height, move_amount);

        default:
            return 0;
    }
}

void Bob::EatFood(std::shared_ptr<Food> p_food, float eat_percentage)
{
    // Eat movePercentage food from the
    energy_t max_can_eat = this->energyMax - this->energyLevel;
    
    energy_t food_energy = (energy_t) (eat_percentage * p_food->getEnergyMax()) / 100;
    if (food_energy > p_food->getEnergy()) {
        food_energy = p_food->getEnergy();
    }
    this->energyLevel += p_food->Consume(std::min(max_can_eat, food_energy));
}

bool Bob::CanEat(std::shared_ptr<Bob> p_bob)
{
    return (this->getID() != p_bob->getID()) && ((p_bob->getMass() / this->getMass()) < Bob::CANNIBALISM_MASS_RATIO);
}

void Bob::EatBob(std::shared_ptr<Bob> p_bob)
{
    energy_t _Be = this->energyLevel;
    energy_t _be = p_bob->getEnergyLevel();
    mass_t _Bm = this->mass;
    mass_t _bm = p_bob->getMass();
    energy_t new_energy = _Be + ((1 / 2.0) * _be) * (1 - (_bm / _Bm));

    this->energyLevel = std::min(new_energy, this->energyMax);
}


void Bob::Reproduce()
{
    this->energyLevel -= Bob::REPRODUCTION_COST;
}

float Bob::MoveRandomly(dimension_t world_width, dimension_t world_height, float move_amount)
{
    if (move_amount != 1) {
        this->movePercentage -= (move_amount * 100);
        return move_amount;
    }

    // Pick a random direction
    RandomNumberGenerator *rng = RandomNumberGenerator::GetInstance();
    MOVE_DIRECTION direction = static_cast<MOVE_DIRECTION>(rng->getRandomNumber(0, static_cast<uint64_t>(MOVE_DIRECTION_COUNT)));

    switch (direction) {
        case UP:
            if (this->y != 0) {
                this->y -= 1;
            }
            break;

        case DOWN:
            if (this->y != std::numeric_limits<position_t>::max() && this->y < static_cast<position_t>(world_height - 1)) {
                this->y += 1;
            }
            break;

        case LEFT:
            if (this->x != 0) {
                this->x -= 1;
            }
            break;

        case RIGHT:
            if (this->x != std::numeric_limits<position_t>::max() && this->x < static_cast<position_t>(world_width - 1)) {
                this->x += 1;
            }
            break;

        default:
            std::cerr << "[ERROR] Unknown direction " << static_cast<int>(direction) << std::endl;
    }

    // Should be 1
    return move_amount;
}

energy_t Bob::CalculateEnergyConsumption(float nb_tiles) const
{
    if (nb_tiles == 0) {
        return (energy_t) 0.5;
    }

    // Energy = Speed²
    energy_t energy_consumption = this->mass * this->velocity * this->velocity;
    return energy_consumption;
}

void Bob::Die()
{
    // std::cout << "[DEBUG] Bob " << this->getID() << " is dead !" << std::endl;
    this->status = DEAD;
}

bool Bob::operator<(const Bob &other) const
{
    return (this->velocity < other.velocity);
}

std::ostream& operator<<(std::ostream& out, const Bob& bob)
{
    out << "bob.id = " << static_cast<int64_t>(bob.getID());
    out << "\tbob.x = " << static_cast<int>(bob.getX());
    out << "\tbob.y = " << static_cast<int>(bob.getY());
    out << "\tbob.energyLevel = " << bob.getEnergyLevel();
    out << "\tbob.velocity = " << bob.getVelocity();
    out << "\tbob.movePercentage = " << bob.getMovePercentage();
    out << "\tbob.mass = " << bob.getMass();
    return out;
}

void Bob::setEnergyLevel(energy_t new_level)
{
    this->energyLevel = new_level;
}

/*****************************/
/*          Getters          */
/*****************************/

Bob::BOB_STATUS Bob::getStatus(void) const
{
    return this->status;
}

energy_t Bob::getEnergyLevel(void) const
{
    return this->energyLevel;
}

energy_t Bob::getEnergyMax(void) const
{
    return this->energyMax;
}

Bob::REPRODUCTION_MODE Bob::getReproductionMode(void) const
{
    return this->reproductionMode;
}

float Bob::getVelocity(void) const
{
    return this->velocity;
}

float Bob::getMovePercentage(void) const
{
    return this->movePercentage;
}

mass_t Bob::getMass(void) const
{
    return this->mass;
}
