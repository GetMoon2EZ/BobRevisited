#pragma once

#include <cstdint>
#include <chrono>
#include <vector>

#include "entities/bob.hpp"
#include "entities/food.hpp"
#include "utils/types.hpp"


class Simulation
{
public:
    // Default values
    static const dimension_t DEFAULT_WORLD_WIDTH = 100;
    static const dimension_t DEFAULT_WORLD_HEIGHT = 100;
    static const uint64_t DEFAULT_WORLD_POPULATION = 100;
    static constexpr std::chrono::milliseconds TICK_DURATION_MS = std::chrono::milliseconds( 100 );
    static const uint8_t TICKS_PER_DAY = 100;
    static const uint16_t FOOD_PER_DAY = 300;

    enum {
        SIMULATION_SUCCESS = 0,
        SIMULATION_ERROR
    };

    Simulation() = default;
    Simulation(dimension_t worldSize);
    Simulation(dimension_t worldWidth, dimension_t worldHeight);

    int Init(uint64_t worldPopulation = DEFAULT_WORLD_POPULATION);
    void Run();
    void Cleanup();

    uint32_t getDay(void) const;
    uint8_t getTick(void) const;
    dimension_t getWorldWidth(void) const;
    dimension_t getWorldHeight(void) const;
    uint64_t getWorldPopulation(void) const;
    std::vector<Bob> getBobs(void) const;
    std::vector<Food> getFoods(void) const;

    ~Simulation() = default;

private:

    // Private members
    bool isRunning = false;
    uint32_t day = 0;
    uint8_t tick = 0;

    dimension_t worldWidth = DEFAULT_WORLD_WIDTH;
    dimension_t worldHeight = DEFAULT_WORLD_HEIGHT;
    uint64_t worldPopulation = DEFAULT_WORLD_POPULATION;
    std::vector<Bob> bobs;
    std::vector<Food> foods;

    void RunTick();
    void UpdateBobsPositions();
    void MakeBobsEat();
    void CheckBobsReproduction();
    void ClearFood();
    void SpawnFood();
};
