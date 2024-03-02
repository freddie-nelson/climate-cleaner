#include "../include/HealthPowerUp.h"

#include <iostream>

Rendering::Texture *HealthPowerUp::s_texture = new Rendering::Texture("assets/images/health-power-up.png");

HealthPowerUp::HealthPowerUp(remi::Engine &engine, glm::vec2 position, float timeToLive)
    : PowerUp(engine, position, PowerUpType::HEALTH_POWERUP, timeToLive)
{
    auto &registry = m_engine.getWorld()->getRegistry();
    auto &material = registry.get<Rendering::Material>(m_entity);
    material.setTexture(s_texture);
}

void HealthPowerUp::apply(Player &player)
{
    PowerUp::apply(player);

    player.heal(HEALTH_POWERUP_HEAL_AMOUNT);
    m_timeToLive = 0;
}

void HealthPowerUp::remove(Player &player)
{
}