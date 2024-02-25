#include "../include/Bullet.h"

#include <remi/Physics/RigidBody2D.h>
#include <remi/Physics/Collider2D.h>
#include <remi/Rendering/Mesh/Mesh.h>
#include <remi/Rendering/Material/Material.h>
#include <remi/Rendering/Renderable.h>
#include <remi/Core/Transform.h>

Bullet::Bullet(remi::Engine &engine, glm::vec2 position, glm::vec2 direction, float speed, float lifeTime, BulletType bulletType)
    : m_engine(engine), m_bulletType(bulletType), m_lifeTime(lifeTime), m_direction(direction), m_speed(speed)
{
    auto &world = *engine.getWorld();
    auto &registry = world.getRegistry();
    auto &sceneGraph = world.getSceneGraph();

    m_entity = registry.create();
    registry.add(m_entity, Rendering::Mesh2D(BULLET_WIDTH, BULLET_HEIGHT));
    registry.add(m_entity, Rendering::Material(Rendering::Color(1.0f, 0.0f, 0.0f, 1.0f)));
    registry.add(m_entity, Rendering::Renderable(true, false));

    auto &transform = registry.add(m_entity, Core::Transform(position));
    transform.setRotation(glm::atan(direction.y, direction.x));

    auto colliderShape = Physics::PolygonColliderShape2D(Rendering::Mesh2D(BULLET_WIDTH, BULLET_HEIGHT));
    auto &collider = registry.add(m_entity, Physics::Collider2D(&colliderShape));

    auto &body = registry.add(m_entity, Physics::RigidBody2D());
    body.setType(Physics::RigidBodyType::DYNAMIC);
    body.setVelocity(direction * speed);
    body.setGravityScale(0.0f);
    body.setFixedRotation(true);
    body.setIsBullet(true);

    body.setBeginContactCallback([this](const Physics::ContactInfo &contactInfo)
                                 { callback(contactInfo); });

    registry.add(m_entity, BulletTag());

    world.addSystem(this);
}

void Bullet::setHitCallback(Physics::RigidBodyCollisionCallback callback)
{
    m_hitCallback = callback;
}

void Bullet::fixedUpdate(World::World &world, const Core::Timestep &timestep)
{
    if (m_lifeTime <= 0)
    {
        return;
    }

    auto &registry = world.getRegistry();

    // keep velocity constant
    auto &body = registry.get<Physics::RigidBody2D>(m_entity);
    body.setVelocity(m_direction * m_speed);

    m_lifeTime -= timestep.getSeconds();

    if (m_lifeTime <= 0)
    {
        world.getRegistry().destroy(m_entity);
        world.removeSystem(this);
        delete this;
    }
}

void Bullet::callback(const Physics::ContactInfo &contactInfo)
{
    if (m_hitCallback != nullptr)
    {
        m_hitCallback(contactInfo);
    }

    auto &world = *m_engine.getWorld();
    auto &registry = world.getRegistry();
    auto &sceneGraph = world.getSceneGraph();

    auto other = contactInfo.entityB;
    if (registry.has<BulletTag>(contactInfo.entityB))
    {
        return;
    }

    registry.destroy(m_entity);
    world.removeSystem(this);
    delete this;
}