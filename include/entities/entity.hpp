#pragma once

#include <cstdint>
#include <iostream>

#include "utils/types.hpp"


class Entity
{
public:
    Entity();
    Entity(position_t x, position_t y);

    bool HasSameCoordinates(const Entity& other) const;

    position_t getX() const;
    position_t getY() const;
    uint64_t getID() const;

    void setX(position_t new_x);
    void setY(position_t new_y);    
    void setCoordinates(position_t new_x, position_t new_y);

    ~Entity() = default;

protected:
    position_t x = 0;
    position_t y = 0;

private:
    uint64_t id;
};
