#include "../include/FastFirePowerUp.h"

#include <iostream>

Rendering::Texture *FastFirePowerUp::s_texture = new Rendering::Texture("assets/images/fast-fire-power-up.png");

FastFirePowerUp::FastFirePowerUp(remi::Engine &engine, glm::vec2 position, float timeToLive)
    : PowerUp(engine, position, PowerUpType::HEALTH_POWERUP, timeToLive)
{
    auto &registry = m_engine.getWorld()->getRegistry();
    auto &material = registry.get<Rendering::Material>(m_entity);
    material.setTexture(s_texture);
}

void FastFirePowerUp::apply(Player &player)
{
    PowerUp::apply(player);

    player.getGun()->setFireRateMultiplier(0.5f);
    m_timeToLive = FAST_FIRE_POWER_UP_DURATION;
}

void FastFirePowerUp::remove(Player &player)
{
    player.getGun()->setFireRateMultiplier(1.0f);
}