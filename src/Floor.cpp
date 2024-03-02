#include "../include/Floor.h"
#include "../include/Layers.h"

#include <remi/Rendering/Mesh/Mesh.h>
#include <remi/Rendering/Material/Material.h>
#include <remi/Rendering/Renderable.h>
#include <remi/Core/Transform.h>
#include <remi/Physics/RigidBody2D.h>
#include <remi/Physics/Collider2D.h>

Floor::Floor(remi::Engine &engine, ECS::Entity player) : m_engine(engine), m_player(player)
{
    createFloor();

    auto &world = *m_engine.getWorld();
    world.addSystem(this);
}

void Floor::update(World::World &world, const Core::Timestep &timestep)
{
    auto &registry = world.getRegistry();
    auto &sceneGraph = world.getSceneGraph();

    auto &playerTransform = registry.get<Core::Transform>(m_player);

    glm::vec2 min = getFloorMin();
    glm::vec2 max = getFloorMax();

    // prevent player from going off floor
    playerTransform.setTranslation(glm::clamp(playerTransform.getTranslation(), min, max));
}

glm::vec2 Floor::getFloorMin() const
{
    float maxX = (SECTIONS_X - 1) * SECTION_WIDTH / 2.0f;
    float maxY = (SECTIONS_Y - 1) * SECTION_HEIGHT / 2.0f;

    glm::vec2 min(-maxX - SECTION_WIDTH, -maxY - SECTION_HEIGHT);
    return min;
}

glm::vec2 Floor::getFloorMax() const
{
    float maxX = (SECTIONS_X - 1) * SECTION_WIDTH / 2.0f;
    float maxY = (SECTIONS_Y - 1) * SECTION_HEIGHT / 2.0f;

    glm::vec2 max(maxX, maxY);
    return max;
}

void Floor::createFloor()
{
    if (m_createdFloor)
    {
        destroyFloor();
    }
    m_createdFloor = true;

    auto &world = *m_engine.getWorld();
    auto &registry = world.getRegistry();
    auto &sceneGraph = world.getSceneGraph();

    // create floor
    m_floor = registry.create();
    registry.add(m_floor, Core::Transform());

    for (int x = 0; x < SECTIONS_X; x++)
    {
        for (int y = 0; y < SECTIONS_Y; y++)
        {
            float sx = (x - SECTIONS_X / 2.0f) * SECTION_WIDTH;
            float sy = (y - SECTIONS_Y / 2.0f) * SECTION_HEIGHT;

            auto floorSection = registry.create();
            auto &t = registry.add(floorSection, Core::Transform(glm::vec2(sx, sy)));
            registry.add(floorSection, Rendering::Mesh2D(SECTION_WIDTH, SECTION_HEIGHT));
            registry.add(floorSection, Rendering::Material(&m_texture));
            registry.add(floorSection, Rendering::Renderable(true, true));
            sceneGraph.relate(m_floor, floorSection);

            t.setZIndex(FLOOR_LAYER);
        }
    }
}

void Floor::destroyFloor()
{
    if (!m_createdFloor)
    {
        return;
    }
    m_createdFloor = false;

    auto &world = *m_engine.getWorld();
    auto &registry = world.getRegistry();
    auto &sceneGraph = world.getSceneGraph();

    auto &children = sceneGraph.children(m_floor);
    for (auto child : children)
    {
        registry.destroy(child);
    }

    registry.destroy(m_floor);
}