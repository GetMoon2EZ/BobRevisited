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

    Bob() = default;
    Bob(uint32_t x, uint32_t y);

    void Move(uint32_t worldWidth, uint32_t worldHeight);
    void Eat(Food &food);

    energy_t getEnergyLevel(void) const;
    uint8_t getHP(void) const;

    ~Bob() = default;
    friend std::ostream& operator<<(std::ostream& os, const Bob& bob);

private:
    energy_t energyLevel = BOB_ENERGY_DEFAULT;
    energy_t energyMax = BOB_ENERGY_MAX;

    MOVE_HEURISTIC moveHeuristic = MOVE_HEURISTIC_RANDOM;

    uint8_t maxHp = 100;
    uint8_t hp = 50;
    uint8_t speed = 1;
    float size = 1;
    uint8_t memory = 0;

    void MoveRandomly(uint32_t worldWidth, uint32_t worldHeight);
    energy_t CalculateEnergyConsumption(uint32_t new_x, uint32_t new_y) const;
};
