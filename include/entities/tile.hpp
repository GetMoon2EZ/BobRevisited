#pragma once

#include <memory>

#include "utils/types.hpp"
#include "entities/bob.hpp"
#include "entities/food.hpp"


class Tile: public Entity
{
public:
    Tile(position_t x, position_t y, std::shared_ptr<Food> p_food = nullptr);

    /**
     * @brief Remove food from tile
     * 
     */
    void RemoveFood();

    void setPFood(std::shared_ptr<Food> p_food);
    std::shared_ptr<Food> getPFood(void) const;

    ~Tile() = default;

private:
    std::shared_ptr<Food> pFood = nullptr;
};
