#pragma once

#include <cstdint>

#include "entities/entity.hpp"
#include "entities/food.hpp"
#include "utils/types.hpp"


class Bob: public Entity
{
public:
    static const energy_t BOB_ENERGY_DEFAULT;
    static const energy_t BOB_ENERGY_MAX;
    static const energy_t REPRODUCTION_COST;
    static const energy_t NEW_BORN_ENERGY;

    typedef enum {
        UP = 0,
        DOWN,
        LEFT,
        RIGHT,
        MOVE_DIRECTION_COUNT
    } MOVE_DIRECTION;

    typedef enum {
        MOVE_HEURISTIC_BRAIN_DEAD,  // Bob does not move
        MOVE_HEURISTIC_RANDOM       // Bob moves randomly
    } MOVE_HEURISTIC;

    typedef enum {
        NO_REPRODUCTION,
        PARTHENOGENESIS
    } REPRODUCTION_MODE;

    Bob() = default;
    Bob(position_t x, position_t y);
    Bob(position_t x, position_t y, energy_t energy);

    void Move(dimension_t worldWidth, dimension_t worldHeight);
    void Eat(Food &food);
    void Reproduce();

    energy_t getEnergyLevel(void) const;
    energy_t getEnergyMax(void) const;
    REPRODUCTION_MODE getReproductionMode(void) const;

    ~Bob() = default;
    friend std::ostream& operator<<(std::ostream& os, const Bob& bob);

private:
    energy_t energyLevel = BOB_ENERGY_DEFAULT;
    energy_t energyMax = BOB_ENERGY_MAX;

    // Behavioral parameters
    MOVE_HEURISTIC moveHeuristic = MOVE_HEURISTIC_RANDOM;
    REPRODUCTION_MODE reproductionMode = PARTHENOGENESIS;

    uint8_t speed = 1;
    float size = 1;
    uint8_t memory = 0;

    void MoveRandomly(dimension_t worldWidth, dimension_t worldHeight);
    energy_t CalculateEnergyConsumption(position_t new_x, position_t new_y) const;
};
