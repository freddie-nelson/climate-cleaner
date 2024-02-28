#pragma once

#include <remi/Engine.h>

#define HEALTH_BAR_WIDTH 0.8f
#define HEALTH_BAR_HEIGHT 0.15f
#define HEALTH_BAR_BORDER_SIZE 0.03f
#define HEALTH_BAR_BACKGROUND_COLOR Rendering::Color(0.0f, 0.1f, 0.0f, 1.0f)
#define HEALTH_BAR_COLOR Rendering::Color(0.0f, 1.0f, 0.0f, 1.0f)

struct HealthBarTag
{
    float health;
    float maxHealth;
};

class HealthBar : public World::System
{
public:
    HealthBar(remi::Engine &engine, ECS::Entity owner, glm::vec2 position);

    void fixedUpdate(World::World &world, const Core::Timestep &timestep) override;

private:
    remi::Engine &m_engine;
    ECS::Entity m_owner;

    ECS::Entity m_container;
    ECS::Entity m_bar;
};