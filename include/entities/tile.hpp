#pragma once

#include <memory>
#include <vector>

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

    void AddBob(std::shared_ptr<Bob> p_bob);
    void RemoveBob(std::shared_ptr<Bob> p_bob);
    std::vector<std::shared_ptr<Bob>> getPBobs(void) const;

    ~Tile() = default;

private:
    std::shared_ptr<Food> pFood = nullptr;
    std::vector<std::shared_ptr<Bob>> pBobs;
};
