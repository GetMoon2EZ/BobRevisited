#include "entities/entity.hpp"

#include <cstdint>

#include "utils/types.hpp"


static uint64_t last_entity_id = 0; 

Entity::Entity()
{
    this->id = ++last_entity_id;
}

Entity::Entity(position_t _x, position_t _y)
{
    this->x = _x;
    this->y = _y;
    this->id = ++last_entity_id;
}

bool Entity::HasSameCoordinates(const Entity& other) const
{
    return this->x == other.getX() && this->y == other.getY();
}


dimension_t Entity::getX() const
{
    return this->x;
}

dimension_t Entity::getY() const
{
    return this->y;
}

uint64_t Entity::getID() const
{
    return this->id;
}

void Entity::setX(position_t new_x)
{
    this->x = new_x;
}

void Entity::setY(position_t new_y)
{
    this->y = new_y;
}

void Entity::setCoordinates(position_t new_x, position_t new_y)
{
    this->setX(new_x);
    this->setY(new_y);
}
