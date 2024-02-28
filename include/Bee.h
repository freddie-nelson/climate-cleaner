#pragma once

#include "Enemy.h"

#define BEE_HEALTH 10.0f
#define BEE_DAMPING 3.0f
#define BEE_DENSITY 1.0f
#define BEE_SPEED 2.0f
#define BEE_MOVE_FORCE 20.0f
#define BEE_WIDTH 0.5f
#define BEE_HEIGHT 0.5f
#define BEE_KNOCKBACK_RESISTANCE 0.1f
#define BEE_COLOR Rendering::Color(0.9f, 0.7f, 0.1f, 1.0f)
#define BEE_HIT_COLOR Rendering::Color(1.0f, 0.0f, 0.0f, 1.0f)
#define BEE_FREEZE_DURATION 0.5f
#define BEE_FREEZE_COLOR Rendering::Color(0.0f, 0.5f, 1.0f, 1.0f)

class Bee : public Enemy
{
public:
    Bee(remi::Engine &engine, ECS::Entity target, glm::vec2 position = glm::vec2(0));

    void fixedUpdate(World::World &world, const Core::Timestep &timestep) override;

    void takeDamage(float damage) override;

    void freeze() override;

protected:
    Rendering::Color m_color = BEE_COLOR;
    float m_freezeTimer = 0.0f;
    float m_health = BEE_HEALTH;
    float m_knockbackResistance = BEE_KNOCKBACK_RESISTANCE;

    float m_hitTime = 0.0f;
    float m_hitDuration = 0.1f;

    void makeEnemy() override;

    void moveEnemy(World::World &world, const Core::Timestep &timestep) override;
};
