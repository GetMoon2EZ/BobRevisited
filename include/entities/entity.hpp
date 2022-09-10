#pragma once

#include <cstdint>
#include <iostream>


class Entity
{
public:
    Entity();
    Entity(uint32_t x, uint32_t y);

    bool HasSameCoordinates(const Entity& other) const;

    uint32_t getX() const;
    uint32_t getY() const;
    uint64_t getID() const;

    void setX(uint32_t new_x);
    void setY(uint32_t new_y);    
    void setCoordinates(uint32_t new_x, uint32_t new_y);

    ~Entity() = default;

protected:
    uint32_t x = 0;
    uint32_t y = 0;

private:
    uint64_t id;
};
