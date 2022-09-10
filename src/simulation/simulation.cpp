#include "simulation/simulation.hpp"

#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <vector>

#include "entities/bob.hpp"
#include "entities/food.hpp"
#include "utils/entity_utils.hpp"
#include "utils/rng.hpp"
#include "display/console_display.hpp"

Simulation::Simulation(uint32_t _worldSize)
{
    this->worldWidth = _worldSize;
    this->worldHeight = _worldSize;
}

Simulation::Simulation(uint32_t _worldWidth, uint32_t _worldHeight)
{
    this->worldWidth = _worldWidth;
    this->worldHeight = _worldHeight;
}

int Simulation::Init(uint64_t _worldPopulation)
{
    std::cout << "Simulation initialization" << std::endl;
    this->worldPopulation = _worldPopulation;

    // Create the bobs
    this->bobs = std::vector<Bob>(this->worldPopulation);
    for (uint64_t i = 0; i < this->worldPopulation; i++) {
        uint32_t x = 0, y = 0;
        getRandomCoordinates(this->worldWidth, this->worldHeight, &x, &y);
        this->bobs[i].setCoordinates(x, y);
        std::cout << "[DEBUG] " << this->bobs[i] << std::endl;
    }

    // Create the food for the first day
    this->SpawnFood();

    this->day = 0;
    this->tick = 0;

    return SIMULATION_SUCCESS;
}

void Simulation::Run()
{
    std::cout << "Simulation running" << std::endl;
    this->isRunning = true;
    this->tick = 1;

    // Simulation loop
    while (this->isRunning) {
        ConsoleDisplay::ClearScreen();
        ConsoleDisplay::DisplayFrame(*this);

        this->RunTick();

        // Stop simulation if every bob is dead
        if (this->worldPopulation == 0)
            this->isRunning = false;

        std::this_thread::sleep_for(TICK_DURATION_MS);
        if (++this->tick == TICKS_PER_DAY) {
            this->tick = 0;
            this->day += 1;
        }
    }
}

void Simulation::Cleanup()
{
    std::cout << "Simulation cleanup" << std::endl;
    RandomNumberGenerator *rng = RandomNumberGenerator::GetInstance();
    rng->Cleanup();
}

void Simulation::RunTick()
{
    this->UpdateBobsPositions();
    this->MakeBobsEat();

    // Spawn food at the start of each day
    if ((this->tick % TICKS_PER_DAY) == 0) {
        this->ClearFood();
        this->SpawnFood();
    }
}

void Simulation::UpdateBobsPositions()
{
    std::vector<Bob> survivors;
    for (Bob &bob: this->bobs) {
        bob.Move(this->worldWidth, this->worldHeight);
        if (bob.getEnergyLevel() > 0) {
            survivors.push_back(bob);
        }
    }

    // Update population
    this->bobs = survivors;
    this->worldPopulation = this->bobs.size();
}

void Simulation::MakeBobsEat()
{
    for (Bob &bob: this->bobs) {
        for (Food &food: this->foods) {
            if (bob.HasSameCoordinates(food)) {
                bob.Eat(food);
            }
        }
    }

    std::vector<Food> remaining_food;
    for (Food &food: this->foods) {
        if (food.getEnergy() > 0) {
            remaining_food.push_back(food);
        }
    }
    this->foods = remaining_food;
}

void Simulation::ClearFood()
{
    this->foods.clear();
}

void Simulation::SpawnFood()
{
    this->foods = std::vector<Food>(FOOD_PER_DAY);

    for (uint64_t i = 0; i < FOOD_PER_DAY; i++) {
        uint32_t food_x = 0, food_y = 0;
        getRandomCoordinates(this->worldWidth, this->worldHeight, &food_x, &food_y);
        this->foods[i].setCoordinates(food_x, food_y);
    }
}

/*****************************/
/*          Getters          */
/*****************************/

uint32_t Simulation::getDay(void) const
{
    return this->day;
}

uint8_t Simulation::getTick(void) const
{
    return this->tick;
}

uint32_t Simulation::getWorldWidth(void) const
{
    return this->worldWidth;
}

uint32_t Simulation::getWorldHeight(void) const
{
    return this->worldHeight;
}

uint64_t Simulation::getWorldPopulation(void) const
{
    return this->worldPopulation;
}

std::vector<Bob> Simulation::getBobs(void) const
{
    return this->bobs;
}

std::vector<Food> Simulation::getFoods(void) const
{
    return this->foods;
}
