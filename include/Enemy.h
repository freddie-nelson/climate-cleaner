#pragma once

#include <remi/Engine.h>

class Enemy : public World::System
{
public:
    Enemy(remi::Engine &engine, ECS::Entity target, glm::vec2 position = glm::vec2(0));

    virtual void fixedUpdate(World::World &world, const Core::Timestep &timestep) override = 0;

    /**
     * Deal damage to the enemy
     *
     * Once the enemy's health reaches 0, the enemy will be destroyed
     *
     * @param damage The amount of damage to deal
     */
    void takeDamage(float damage);

    void knockback(glm::vec2 direction, float force);

    bool isDead() const { return m_health <= 0; }

protected:
    remi::Engine &m_engine;
    ECS::Entity m_target;
    glm::vec2 m_startPos;

    float m_health = 10.0f;
    float m_knockbackResistance = 0.0f;

    ECS::Entity m_enemy;

    virtual void makeEnemy() = 0;

    virtual void moveEnemy(World::World &world, const Core::Timestep &timestep) = 0;
};

struct EnemyTag
{
    Enemy *enemy;
};