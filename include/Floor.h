#pragma once

#include <remi/Engine.h>
#include <glm/glm.hpp>

#define FLOOR_WIDTH 25.0f
#define FLOOR_HEIGHT 25.0f
#define FLOOR_FRICTION 1.0f

class Floor
{
public:
    Floor(remi::Engine &engine, glm::vec2 position = glm::vec2(0));

private:
    ECS::Entity m_floor;
};