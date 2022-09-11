#pragma once

#include <cstdint>
#include <chrono>
#include <vector>
#include <memory>

#include "entities/tile.hpp"
#include "entities/bob.hpp"
#include "entities/food.hpp"
#include "utils/types.hpp"


class Simulation
{
public:
    // Default values
    static const dimension_t DEFAULT_WORLD_WIDTH;
    static const dimension_t DEFAULT_WORLD_HEIGHT;
    static const uint64_t DEFAULT_WORLD_POPULATION;
    static const std::chrono::milliseconds TICK_DURATION_MS;
    static const uint8_t TICKS_PER_DAY;
    static const uint16_t FOOD_PER_DAY;

    static const float VELOCITY_MUTATION_RATE; 

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
    std::vector<std::shared_ptr<Bob>> getBobs(void) const;
    std::vector<std::shared_ptr<Food>> getFoods(void) const;

    ~Simulation() = default;

private:
    bool isRunning = false;
    uint32_t day = 0;
    uint8_t tick = 0;
    std::vector<std::vector<Tile>> tiles;

    dimension_t worldWidth = DEFAULT_WORLD_WIDTH;
    dimension_t worldHeight = DEFAULT_WORLD_HEIGHT;
    uint64_t worldPopulation = DEFAULT_WORLD_POPULATION;
    std::vector<std::shared_ptr<Bob>> bobs;
    // Store pointers to food to share the values with tiles
    std::vector<std::shared_ptr<Food>> foods;

    void RunTick();
    void UpdateBobsPositions();
    void CheckBobReproduction(std::shared_ptr<Bob> bob);
    void CleanupDeadBobs();
    void CleanupEmptyFoods();
    void ClearFood();
    void SpawnFood();
};
