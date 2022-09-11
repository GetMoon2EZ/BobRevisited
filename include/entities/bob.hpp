#pragma once

#include <cstdint>
#include <memory>

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
    static const float BOB_VELOCITY_DEFAULT;

    typedef enum {
        ALIVE,
        DEAD
    } BOB_STATUS;

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
    Bob(position_t x, position_t y, energy_t energy, float velocity);

    /**
     * @brief Try to move through "move_amount" tiles in the world
     * 
     * @param worldWidth Width of the world
     * @param worldHeight Height of the world
     * @param move_amount Maximum amount of tiles to try to traverse
     * @return float The number of tiles moved (value between 0 and move_amount)
     */
    float Move(dimension_t worldWidth, dimension_t worldHeight, float move_amount);

    /**
     * @brief Eat a percentage of the food
     * 
     * @param food Food to eat (must be in the same tile)
     * @param EatPercentage Percentage of the available food to eat [0-100]
     */
    void Eat(std::shared_ptr<Food> p_food, float eat_percentage);
    void Reproduce();
    energy_t CalculateEnergyConsumption(float nb_tiles) const;
    void Die();

    BOB_STATUS getStatus(void) const;
    energy_t getEnergyLevel(void) const;
    energy_t getEnergyMax(void) const;
    REPRODUCTION_MODE getReproductionMode(void) const;
    float getVelocity(void) const;
    float getMovePercentage(void) const;

    void setEnergyLevel(energy_t new_level);

    ~Bob() = default;

    friend std::ostream& operator<<(std::ostream& os, const Bob& bob);
    bool operator<(const Bob &other) const;

private:
    BOB_STATUS status = ALIVE; 
    energy_t energyLevel = BOB_ENERGY_DEFAULT;
    energy_t energyMax = BOB_ENERGY_MAX;

    // Behavioral parameters
    MOVE_HEURISTIC moveHeuristic = MOVE_HEURISTIC_RANDOM;
    REPRODUCTION_MODE reproductionMode = PARTHENOGENESIS;

    /*
    Solution 1:
        * velocity member value which indicates bob's velocity
        * precise_x and precise_y which indicate bob's position with precision
    For each tick:
        * Call bob.Move
            * If precise position is not INTEGERS (ie: bob is between 2 tiles)
                * Consume the rest of the tile
            * Else
                * Move normally
    
    Solution 2:
        * velocity member value which indicates bob's velocity
        * move_percentage
    For each tick:
        * Call bob.Move(move_amount)
            * if (move_percentage != 100)
                * EAT move_percentage
                * move_percentage = 100
                * return old_move_percentage
            * else
            *   * move
    */
    float velocity = BOB_VELOCITY_DEFAULT;
    float movePercentage = 100;

    float size = 1;
    uint8_t memory = 0;

    float MoveRandomly(dimension_t worldWidth, dimension_t worldHeight, float move_amount);
};
