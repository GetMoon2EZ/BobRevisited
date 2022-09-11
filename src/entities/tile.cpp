#include "entities/tile.hpp"

#include <memory>

#include "utils/types.hpp"
#include "entities/entity.hpp"
#include "entities/bob.hpp"
#include "entities/food.hpp"


Tile::Tile(position_t _x, position_t _y, std::shared_ptr<Food> p_food): Entity(_x, _y)
{
    this->pFood = p_food;
}

void Tile::RemoveFood(void)
{
    this->pFood = nullptr;
}

std::shared_ptr<Food> Tile::getPFood(void) const
{
    return this->pFood;
}

void Tile::setPFood(std::shared_ptr<Food> p_food)
{
    this->pFood = p_food;
}
