#pragma once

#include "simulation/simulation.hpp"


class ConsoleDisplay
{
public:
    static void ClearScreen();
    static void DisplayFrame(Simulation sim);
};
