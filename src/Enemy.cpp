#include "../include/Enemy.h"

#include <remi/Physics/RigidBody2d.h>

Enemy::Enemy(remi::Engine &engine, ECS::Entity target, glm::vec2 position)
    : m_engine(engine), m_target(target), m_startPos(position)
{
    auto &world = *m_engine.getWorld();
    world.addSystem(this);
}

void Enemy::takeDamage(float damage)
{
    m_health -= damage;

    if (m_health <= 0)
    {
        auto &world = *m_engine.getWorld();
        auto &registry = world.getRegistry();

        world.removeSystem(this);
        registry.destroy(m_enemy);
    }
}

void Enemy::knockback(glm::vec2 direction, float force)
{
    auto &world = *m_engine.getWorld();
    auto &registry = world.getRegistry();

    auto &body = registry.get<Physics::RigidBody2D>(m_enemy);

    body.applyForce(direction * force * (1.0f - m_knockbackResistance));
}
