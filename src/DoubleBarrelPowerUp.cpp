#include "../include/DoubleBarrelPowerUp.h"

#include <iostream>

Rendering::Texture *DoubleBarrelPowerUp::s_texture = new Rendering::Texture("assets/images/double-barrel-power-up.png");

DoubleBarrelPowerUp::DoubleBarrelPowerUp(remi::Engine &engine, glm::vec2 position, float timeToLive)
    : PowerUp(engine, position, PowerUpType::HEALTH_POWERUP, timeToLive)
{
    auto &registry = m_engine.getWorld()->getRegistry();
    auto &material = registry.get<Rendering::Material>(m_entity);
    material.setTexture(s_texture);
}

void DoubleBarrelPowerUp::apply(Player &player)
{
    PowerUp::apply(player);

    player.getGun()->setBulletMultiplier(2);
    m_timeToLive = DOUBLE_BARREL_POWERUP_DURATION;
}

void DoubleBarrelPowerUp::remove(Player &player)
{
    player.getGun()->setBulletMultiplier(1);
}