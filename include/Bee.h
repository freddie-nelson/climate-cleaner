#pragma once

#include "Enemy.h"
#include "HealthBar.h"

#define BEE_HEALTH 10.0f
#define BEE_DAMPING 3.0f
#define BEE_DENSITY 1.0f
#define BEE_SPEED 2.5f
#define BEE_MOVE_FORCE 20.0f
#define BEE_WIDTH 1.0f
#define BEE_HEIGHT 1.0f
#define BEE_KNOCKBACK_RESISTANCE 0.1f
#define BEE_COLOR Rendering::Color(1.0f, 1.0f, 1.0f, 1.0f)
#define BEE_HIT_COLOR Rendering::Color(1.0f, 0.0f, 0.0f, 1.0f)
#define BEE_FREEZE_DURATION 0.5f
#define BEE_FREEZE_COLOR Rendering::Color(0.0f, 0.5f, 1.0f, 1.0f)
#define BEE_SHADOW_POSITION glm::vec2(0.0f, -BEE_HEIGHT * 0.7f)
#define BEE_SHADOW_SCALE glm::vec2(0.5f, 0.25f)

class Bee : public Enemy
{
public:
    Bee(remi::Engine &engine, ECS::Entity target, glm::vec2 position = glm::vec2(0), int wave = 1);

    void fixedUpdate(World::World &world, const Core::Timestep &timestep) override;

    void takeDamage(float damage) override;

    void freeze() override;

protected:
    static Rendering::Texture texture;

    ECS::Entity m_shadow;

    HealthBar *m_healthBar = nullptr;

    Rendering::Color m_color = BEE_COLOR;

    float m_attackRange = 1.35f;

    float m_freezeTimer = 0.0f;
    float m_health = BEE_HEALTH;
    float m_knockbackResistance = BEE_KNOCKBACK_RESISTANCE;

    float m_hitTime = 0.0f;
    float m_hitDuration = 0.1f;

    void makeEnemy() override;

    void moveEnemy(World::World &world, const Core::Timestep &timestep) override;
};
