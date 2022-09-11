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
    static const float BOB_DEFAULT_VELOCITY;
    static const mass_t BOB_DEFAULT_MASS;
    static const float CANNIBALISM_MASS_RATIO;


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
    Bob(position_t x, position_t y, energy_t energy, float velocity, float mass);

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
    void EatFood(std::shared_ptr<Food> p_food, float eat_percentage);
    bool CanEat(std::shared_ptr<Bob> p_bob);
    void EatBob(std::shared_ptr<Bob> p_bob);
    void Reproduce();
    energy_t CalculateEnergyConsumption(float nb_tiles) const;
    void Die();

    BOB_STATUS getStatus(void) const;
    energy_t getEnergyLevel(void) const;
    energy_t getEnergyMax(void) const;
    REPRODUCTION_MODE getReproductionMode(void) const;
    float getVelocity(void) const;
    float getMovePercentage(void) const;
    mass_t getMass(void) const;

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

    float velocity = BOB_DEFAULT_VELOCITY;
    float movePercentage = 100;

    mass_t mass = BOB_DEFAULT_MASS;
    uint8_t memory = 0;

    float MoveRandomly(dimension_t worldWidth, dimension_t worldHeight, float move_amount);
};
