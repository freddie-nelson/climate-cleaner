#include "../include/Bee.h"

#include <remi/Physics/RigidBody2d.h>
#include <remi/Physics/Collider2d.h>
#include <remi/Rendering/Renderable.h>
#include <remi/Rendering/Mesh/Mesh.h>
#include <remi/Rendering/Material/Material.h>

Bee::Bee(remi::Engine &engine, ECS::Entity target, glm::vec2 position)
    : Enemy(engine, target, position)
{
    makeEnemy();
}

void Bee::fixedUpdate(World::World &world, const Core::Timestep &timestep)
{
    // hit material change
    m_hitTime -= timestep.getSeconds();
    if (m_hitTime < 0)
    {
        m_hitTime = 0;
    }

    auto &registry = world.getRegistry();

    auto &material = registry.get<Rendering::Material>(m_enemy);
    material.setColor(m_hitTime > 0 ? BEE_HIT_COLOR : BEE_COLOR);

    // freeze
    if (m_frozen)
    {
        m_freezeTimer -= timestep.getSeconds();
        if (m_freezeTimer < 0)
        {
            m_frozen = false;
            m_freezeTimer = 0;
        }

        material.setColor(m_freezeTimer > 0 ? BEE_FREEZE_COLOR : BEE_COLOR);
    }

    moveEnemy(world, timestep);
    handleAttack(world, timestep);
}

void Bee::freeze()
{
    Enemy::freeze();

    m_freezeTimer = BEE_FREEZE_DURATION;
}

void Bee::takeDamage(float damage)
{
    m_hitTime = m_hitDuration;
    m_color = BEE_HIT_COLOR;

    Enemy::takeDamage(damage);
}

void Bee::makeEnemy()
{
    auto &world = *m_engine.getWorld();
    auto &registry = world.getRegistry();
    auto &sceneGraph = world.getSceneGraph();

    m_enemy = registry.create();
    registry.add<Core::Transform>(m_enemy, Core::Transform(m_startPos));
    registry.add<EnemyTag>(m_enemy, EnemyTag{this});

    auto &mesh = registry.add<Rendering::Mesh2D>(m_enemy, Rendering::Mesh2D(BEE_WIDTH, BEE_HEIGHT));
    registry.add<Rendering::Material>(m_enemy, Rendering::Material(m_color));
    registry.add(m_enemy, Rendering::Renderable(true, false));

    auto shape = Physics::PolygonColliderShape2D(mesh);
    auto &collider = registry.add<Physics::Collider2D>(m_enemy, Physics::Collider2D(&shape));
    collider.setDensity(BEE_DENSITY);

    auto &body = registry.add<Physics::RigidBody2D>(m_enemy, Physics::RigidBody2D());
    body.setLinearDamping(BEE_DAMPING);
}

void Bee::moveEnemy(World::World &world, const Core::Timestep &timestep)
{
    auto &registry = world.getRegistry();
    auto &sceneGraph = world.getSceneGraph();

    auto &body = registry.get<Physics::RigidBody2D>(m_enemy);
    auto &transform = registry.get<Core::Transform>(m_enemy);

    auto targetTransform = Core::Transform(sceneGraph.getModelMatrix(m_target));
    auto beeTransform = Core::Transform(sceneGraph.getModelMatrix(m_enemy));

    auto direction = glm::normalize(targetTransform.getTranslation() - beeTransform.getTranslation());
    body.applyForce(direction * BEE_MOVE_FORCE);

    auto velocity = body.getVelocity();
    if (glm::length(velocity) > BEE_SPEED)
    {
        body.setVelocity(glm::normalize(velocity) * BEE_SPEED);
    }

    if (direction.x < 0)
    {
        transform.setScale(glm::vec2(-1, 1));
    }
    else
    {
        transform.setScale(glm::vec2(1, 1));
    }
}