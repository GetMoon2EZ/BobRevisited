#include "display/console_display.hpp"

#include <iostream>
#include <vector>
#include <memory>

#include "simulation/simulation.hpp"
#include "entities/bob.hpp"
#include "entities/food.hpp"
#include "utils/types.hpp"


void ConsoleDisplay::ClearScreen()
{
    for (int i = 0; i < 25; i++) {
        std::cout << std::endl;
    }
}

void ConsoleDisplay::DisplayFrame(Simulation sim)
{
    dimension_t width = sim.getWorldWidth();
    dimension_t height = sim.getWorldHeight();

    // worldHeight lines of worldWidth*2 chars + worldHeight of '\n'
    std::string buffer((height * width*2) + height - 1, '_');

    // Draw the grid in the buffer
    for (dimension_t y = 0; y < height; y++) {
        for (dimension_t x = 0; x < width; x++) {
            buffer[y * height * 2 + y + x * 2] = '|';
        }
        if (y)
            buffer[(y * height) * 2 + y - 1] = '\n';
    }

    // Draw a F for each piece of food
    const std::vector<std::shared_ptr<Food>> p_foods = sim.getFoods();
    for (std::shared_ptr<Food> p_food: p_foods) {
        buffer[(p_food->getY() * height) * 2 + p_food->getY() + p_food->getX() * 2 + 1] = 'F';
    }

    // Draw a B for each bob in the grid
    const std::vector<std::shared_ptr<Bob>> p_bobs = sim.getBobs();
    for (const std::shared_ptr<Bob> &p_bob: p_bobs) {
        std::cout << *p_bob << std::endl;
        buffer[(p_bob->getY() * height) * 2 + p_bob->getY() + p_bob->getX() * 2 + 1] = 'B';
    }

    std::cout << buffer << std::endl;
    std::cout << "Current population: " << sim.getWorldPopulation() << std::endl;
    std::cout << "Day: " << static_cast<int>(sim.getDay());
    std::cout << " (" << static_cast<int>(sim.getTick()) << "/" << static_cast<int>(sim.TICKS_PER_DAY) << ")" << std::endl;
}
