#include "entities/tile.hpp"

#include <memory>
#include <vector>

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

void Tile::AddBob(std::shared_ptr<Bob> p_bob)
{
    this->pBobs.push_back(p_bob);
}

void Tile::RemoveBob(std::shared_ptr<Bob> p_bob)
{
    std::vector<std::shared_ptr<Bob>> tmp;
    for (std::shared_ptr<Bob> i_bob: this->pBobs) {
        if (i_bob->getID() != p_bob->getID()) {
            tmp.push_back(i_bob);
        }
    }

    this->pBobs = tmp;
}

std::vector<std::shared_ptr<Bob>> Tile::getPBobs(void) const
{
    return this->pBobs;
}
