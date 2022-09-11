#include "simulation/simulation.hpp"

#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <utility>
#include <memory>
#include <algorithm>

#include "entities/entity.hpp"
#include "entities/bob.hpp"
#include "entities/food.hpp"
#include "utils/entity_utils.hpp"
#include "utils/rng.hpp"
#include "display/console_display.hpp"


/*****************************/
/*   Simulation Parameters   */
/*****************************/

const dimension_t Simulation::DEFAULT_WORLD_WIDTH = 100;
const dimension_t Simulation::DEFAULT_WORLD_HEIGHT = 100;
const uint64_t Simulation::DEFAULT_WORLD_POPULATION = 100;
const std::chrono::milliseconds Simulation::TICK_DURATION_MS = std::chrono::milliseconds( 200 );
const uint8_t Simulation::TICKS_PER_DAY = 100;
const uint16_t Simulation::FOOD_PER_DAY = 300;

const float Simulation::VELOCITY_MUTATION_RATE = 0.0F; 
const mass_t Simulation::MASS_MUTATION_RATE = 0.1F;

/*****************************/
/*       Constructors        */
/*****************************/

Simulation::Simulation(dimension_t _worldSize)
{
    this->worldWidth = _worldSize;
    this->worldHeight = _worldSize;
}

Simulation::Simulation(dimension_t _worldWidth, dimension_t _worldHeight)
{
    this->worldWidth = _worldWidth;
    this->worldHeight = _worldHeight;
}

/*****************************/
/*       Public Methods      */
/*****************************/

int Simulation::Init(uint64_t _worldPopulation)
{
    std::cout << "Simulation initialization" << std::endl;
    
    // Create the tiles
    this->tiles = std::vector<std::vector<Tile>>();
    for (dimension_t x = 0; x < this->worldWidth; x++) {
        this->tiles.push_back(std::vector<Tile>());
        for (dimension_t y = 0; y < this->worldHeight; y++) {
            this->tiles[x].push_back(Tile(x, y));
        }
    }

    // Create the bobs
    this->worldPopulation = _worldPopulation;
    this->bobs = std::vector<std::shared_ptr<Bob>>();
    for (uint64_t i = 0; i < this->worldPopulation; i++) {
        position_t bob_x = 0, bob_y = 0;
        getRandomCoordinates(this->worldWidth, this->worldHeight, &bob_x, &bob_y);
        std::shared_ptr<Bob> p_bob = std::make_shared<Bob>(bob_x, bob_y);
        if (p_bob) {
            this->tiles[bob_x][bob_y].AddBob(p_bob);
            this->bobs.push_back(std::move(p_bob));
            std::cout << "[DEBUG] " << *(this->bobs[i]) << std::endl;
        }
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
    // Bobs need to be sorted by speed
    // std::sort(this->bobs.begin(), this->bobs.end());

    this->UpdateBobsPositions();

    this->CheckBobCannibalism();

    // Cleanup dead or empty entities
    this->CleanupDeadBobs();
    this->CleanupEmptyFoods();

    // Clear old food and spawn new food at the start of each day
    if ((this->tick % TICKS_PER_DAY) == 0) {
        this->ClearFood();
        this->SpawnFood();
    }
}

/*****************************/
/*      Private Methods      */
/*****************************/

void Simulation::UpdateBobsPositions()
{
    for (std::shared_ptr<Bob> p_bob: this->bobs) {
        if (!p_bob)
            continue;
        // Save position
        position_t old_x = p_bob->getX(), old_y = p_bob->getY();
        // Total amount moved, needed for energy consumption calculation
        float total_moved = 0;
        // Maximum number of moves (equal to velocity)
        float nb_move = p_bob->getVelocity();
        while (nb_move > 0) { // While Bob can move during current tick
            // std::cout << "[DEBUG] " << *p_bob << std::endl;

            float moved = 0;
            float move_amount = std::min(nb_move, 1.0F);
            // std::cout << "[DEBUG] move_amount = " << move_amount << std::endl;

            // Try to move by the current move amount
            // Move will return the actual amount that bob has moved
            moved = p_bob->Move(this->worldWidth, this->worldHeight, move_amount);

            Tile new_tile = this->tiles[p_bob->getX()][p_bob->getY()];

            // If there is food on the tile then eat it
            std::shared_ptr<Food> p_food = new_tile.getPFood();
            if (p_food != nullptr) {
                p_bob->EatFood(p_food, moved * 100);
            }

            total_moved += moved;
            nb_move -= move_amount;
        }

        // Update tiles bob arrays
        this->tiles[old_x][old_y].RemoveBob(p_bob);
        this->tiles[p_bob->getX()][p_bob->getY()].AddBob(p_bob);

        // std::cout << "[DEBUG] " << *p_bob << std::endl;
        this->CheckBobReproduction(p_bob);

        // std::cout << "[DEBUG] " << bob << std::endl;
        // std::cout << "[DEBUG] " << total_moved << std::endl;
        // Energy is updated after every move is done
        energy_t consumed = p_bob->CalculateEnergyConsumption(total_moved);
        // std::cout << "[DEBUG] consumed = " << consumed << std::endl; 
        if (consumed < p_bob->getEnergyLevel()) {
            p_bob->setEnergyLevel(p_bob->getEnergyLevel() - consumed);
        } else {
            this->Kill(p_bob);
        }
    }
}

void Simulation::CheckBobReproduction(std::shared_ptr<Bob> p_bob)
{
    RandomNumberGenerator *rng = RandomNumberGenerator::GetInstance();

    switch (p_bob->getReproductionMode()) {
        case Bob::NO_REPRODUCTION:
            break;
        case Bob::PARTHENOGENESIS:
            // Reproduction condition: bob is at max energy
            if (p_bob->getEnergyLevel() == p_bob->getEnergyMax()) {
                p_bob->Reproduce();
                float velocity = rng->getRandomFloat(p_bob->getVelocity() - VELOCITY_MUTATION_RATE, p_bob->getVelocity() + VELOCITY_MUTATION_RATE);
                mass_t mass = rng->getRandomFloat(p_bob->getMass() - MASS_MUTATION_RATE, p_bob->getMass() + MASS_MUTATION_RATE);
                // std::cout << "[DEBUG] New BOB spawned from bob " << p_bob->getID() << std::endl; 
                std::shared_ptr<Bob> p_new_bob = std::make_shared<Bob>(p_bob->getX(), p_bob->getY(), Bob::NEW_BORN_ENERGY, velocity, mass);
                if (p_new_bob) {
                    this->tiles[p_new_bob->getX()][p_new_bob->getY()].AddBob(p_new_bob);
                    this->bobs.push_back(std::move(p_new_bob));
                }
            }
            break;
    }

    // Update world population
    this->worldPopulation = this->bobs.size();
}

void Simulation::CheckBobCannibalism()
{
    for (std::shared_ptr<Bob> p_bob_l: this->bobs) {
        Tile tile = this->tiles[p_bob_l->getX()][p_bob_l->getY()];
        for (std::shared_ptr<Bob> p_bob_s: tile.getPBobs()) {
            if (p_bob_l->CanEat(p_bob_s)) {
                p_bob_l->EatBob(p_bob_s);
                this->Kill(p_bob_s);
            }
        }
    }
}

void Simulation::Kill(std::shared_ptr<Bob> p_bob)
{
    this->tiles[p_bob->getX()][p_bob->getY()].RemoveBob(p_bob);
    p_bob->Die();
}

void Simulation::CleanupDeadBobs()
{
    this->bobs.erase(
        std::remove_if(
            this->bobs.begin(),
            this->bobs.end(),
            [](std::shared_ptr<Bob> p_bob){
                return !p_bob || p_bob->getStatus() != Bob::ALIVE;
            }
        ),
        this->bobs.end());

    this->worldPopulation = bobs.size();
}

void Simulation::CleanupEmptyFoods()
{
    std::vector<std::shared_ptr<Food>> tmp;
    for (std::shared_ptr<Food> p_food: this->foods) {
        if (p_food->getEnergy() > 0) {
            tmp.push_back(p_food);
        } else {
            this->tiles[p_food->getX()][p_food->getY()].RemoveFood();
        }
    }
    this->foods = tmp;
}

void Simulation::ClearFood()
{
    // Remove food from tiles
    for (std::shared_ptr<Food> p_food: this->foods) {
        this->tiles[p_food->getX()][p_food->getY()].RemoveFood();
    }
    // Clear food vector
    this->foods.clear();
}

void Simulation::SpawnFood()
{
    for (uint64_t i = 0; i < FOOD_PER_DAY; i++) {
        position_t food_x = 0, food_y = 0;
        getRandomCoordinates(this->worldWidth, this->worldHeight, &food_x, &food_y);

        std::shared_ptr<Food> p_tile_food = this->tiles[food_x][food_y].getPFood();
        if (p_tile_food != nullptr) {
            // Add the food to the tile
            p_tile_food->AddEnergy(Food::FOOD_DEFAULT_ENERGY);
        } else {
            // Create a new food pointer
            std::shared_ptr<Food> p_food = std::make_shared<Food>(food_x, food_y);
            this->tiles[food_x][food_y].setPFood(p_food);
            this->foods.push_back(std::move(p_food));
        }
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

dimension_t Simulation::getWorldWidth(void) const
{
    return this->worldWidth;
}

dimension_t Simulation::getWorldHeight(void) const
{
    return this->worldHeight;
}

uint64_t Simulation::getWorldPopulation(void) const
{
    return this->worldPopulation;
}

std::vector<std::shared_ptr<Bob>> Simulation::getBobs(void) const
{
    return this->bobs;
}

std::vector<std::shared_ptr<Food>> Simulation::getFoods(void) const
{
    return this->foods;
}
