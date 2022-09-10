#include "display/console_display.hpp"

#include <iostream>
#include <vector>

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
    const std::vector<Food> &foods = sim.getFoods();
    for (const Food &food: foods) {
        buffer[(food.getY() * height) * 2 + food.getY() + food.getX() * 2 + 1] = 'F';
    }

    // Draw a B for each bob in the grid
    const std::vector<Bob> &bobs = sim.getBobs();
    for (const Bob &bob: bobs) {
        buffer[(bob.getY() * height) * 2 + bob.getY() + bob.getX() * 2 + 1] = 'B';
    }

    std::cout << buffer << std::endl;
    std::cout << "Current population: " << sim.getBobs().size() << std::endl;
    std::cout << "Day: " << static_cast<int>(sim.getDay());
    std::cout << " (" << static_cast<int>(sim.getTick()) << "/" << static_cast<int>(sim.TICKS_PER_DAY) << ")" << std::endl;
}
