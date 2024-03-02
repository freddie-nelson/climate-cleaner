#include "../include/Bullet.h"
#include "../include/Enemy.h"
#include "../include/Layers.h"

#include <stdexcept>
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
    transform.setZIndex(BULLET_LAYER);

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
    auto &registry = world.getRegistry();

    if (m_exploding)
    {
        if (!m_createdExplosion)
        {
            m_explosion = registry.create();
            registry.add(m_explosion, Core::Transform(registry.get<Core::Transform>(m_entity).getTranslation()));
            registry.add(m_explosion, Rendering::Mesh2D(EXPLODING_BULLET_STARTING_RADIUS, 32u));
            registry.add(m_explosion, Rendering::Material(EXPLODING_BULLET_COLOR));
            registry.add(m_explosion, Rendering::Renderable(true, false));

            auto pos = registry.get<Core::Transform>(m_entity).getTranslation();

            float finalRadius = EXPLODING_BULLET_RADIUS;
            auto &physicsWorld = *m_engine.getPhysicsWorld();

            auto entities = physicsWorld.query(Core::BoundingCircle(pos, finalRadius));
            for (auto e : entities)
            {
                if (registry.has<BulletTag>(e) || !registry.has<EnemyTag>(e))
                {
                    continue;
                }

                auto &body = registry.get<Physics::RigidBody2D>(e);
                auto &t = registry.get<Core::Transform>(e);

                auto dir = glm::normalize(t.getTranslation() - pos);
                body.applyForce(dir * static_cast<float>(EXPLODING_BULLET_FORCE));

                auto &enemy = *registry.get<EnemyTag>(e).enemy;
                enemy.knockback(dir, getBulletKnockback());
                enemy.takeDamage(getBulletDamage());
            }

            registry.destroy(m_entity);
            m_createdExplosion = true;
        }

        auto &transform = registry.get<Core::Transform>(m_explosion);
        transform.setZIndex(10);
        transform.scale(EXPLODING_BULLET_SPEED);

        if (transform.getScale().x >= EXPLODING_BULLET_RADIUS / EXPLODING_BULLET_STARTING_RADIUS)
        {
            registry.destroy(m_explosion);
            world.removeSystem(this);
            delete this;
        }

        return;
    }

    if (m_lifeTime <= 0)
    {
        return;
    }

    // keep velocity constant
    auto &body = registry.get<Physics::RigidBody2D>(m_entity);
    body.setVelocity(m_direction * m_speed);

    m_lifeTime -= timestep.getSeconds();

    if (m_lifeTime <= 0 && !m_exploding)
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

    if (!registry.has<Physics::RigidBody2D>(m_entity))
    {
        return;
    }

    auto &body = registry.get<Physics::RigidBody2D>(m_entity);

    if (registry.has<EnemyTag>(contactInfo.entityB))
    {
        auto &enemy = *registry.get<EnemyTag>(contactInfo.entityB).enemy;
        enemy.knockback(glm::normalize(body.getVelocity()), getBulletKnockback());
        enemy.takeDamage(getBulletDamage());

        if (m_bulletType == FREEZE_BULLET)
        {
            enemy.freeze();
        }
    }

    if (m_bulletType == EXPLODING_BULLET)
    {
        m_exploding = true;
    }
    else
    {
        registry.destroy(m_entity);
        world.removeSystem(this);
        delete this;
    }
}

float Bullet::getBulletDamage() const
{
    switch (m_bulletType)
    {
    case BULLET:
        return 2.5f;
    case FREEZE_BULLET:
        return 3.0f;
    case EXPLODING_BULLET:
        return 4.0f;
    default:
        throw std::runtime_error("Invalid bullet type");
    }
}

float Bullet::getBulletKnockback() const
{
    switch (m_bulletType)
    {
    case BULLET:
        return 1000.0f;
    case FREEZE_BULLET:
        return 250.0f;
    case EXPLODING_BULLET:
        return 500.0f;
    default:
        throw std::runtime_error("Invalid bullet type");
    }
}