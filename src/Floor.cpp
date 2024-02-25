#include "../include/Floor.h"

#include <remi/Rendering/Mesh/Mesh.h>
#include <remi/Rendering/Material/Material.h>
#include <remi/Rendering/Renderable.h>
#include <remi/Core/Transform.h>
#include <remi/Physics/RigidBody2D.h>
#include <remi/Physics/Collider2D.h>

Floor::Floor(remi::Engine &engine, glm::vec2 position)
{
    auto &world = *engine.getWorld();
    auto &registry = world.getRegistry();
    auto &sceneGraph = world.getSceneGraph();

    // create floor
    m_floor = registry.create();
    registry.add(m_floor, Core::Transform(position));

    auto &body = registry.add(m_floor, Physics::RigidBody2D());
    body.setType(Physics::RigidBodyType::STATIC);

    auto colliderShape = Physics::PolygonColliderShape2D(Rendering::Mesh2D(FLOOR_WIDTH, FLOOR_HEIGHT));
    auto &collider = registry.add(m_floor, Physics::Collider2D(&colliderShape));
    collider.setFriction(FLOOR_FRICTION);

    registry.add(m_floor, Rendering::Mesh2D(FLOOR_WIDTH, FLOOR_HEIGHT));
    registry.add(m_floor, Rendering::Material(Rendering::Color(0, 0.4f, 0.0f, 1.0f)));
    registry.add(m_floor, Rendering::Renderable(true, true));
}