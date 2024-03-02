#pragma once

#include <remi/Engine.h>
#include <glm/glm.hpp>

#define SECTIONS_X 15
#define SECTIONS_Y 15

#define SECTION_WIDTH 5.0f
#define SECTION_HEIGHT 5.0f

class Floor : public World::System
{
public:
    Floor(remi::Engine &engine, ECS::Entity player);

    void update(World::World &world, const Core::Timestep &timestep) override;

    void createFloor();
    void destroyFloor();

    glm::vec2 getFloorMin() const;
    glm::vec2 getFloorMax() const;

private:
    Rendering::Texture m_texture = Rendering::Texture("assets/images/floor.png");

    remi::Engine &m_engine;
    ECS::Entity m_player;

    ECS::Entity m_floor;

    bool m_createdFloor = false;
};