#include <iostream>

#include "simulation/simulation.hpp"


int main()
{
    Simulation simulation = Simulation();
    simulation.Init();
    simulation.Run();
    simulation.Cleanup();
    return 0;
}
