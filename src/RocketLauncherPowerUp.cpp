#include "../include/RocketLauncherPowerUp.h"

#include <iostream>

Rendering::Texture *RocketLauncherPowerUp::s_texture = new Rendering::Texture("assets/images/rocket-launcher.png");

RocketLauncherPowerUp::RocketLauncherPowerUp(remi::Engine &engine, glm::vec2 position, float timeToLive)
    : PowerUp(engine, position, PowerUpType::ROCKET_LAUNCHER_POWERUP, timeToLive)
{
    auto &registry = m_engine.getWorld()->getRegistry();
    auto &material = registry.get<Rendering::Material>(m_entity);
    material.setTexture(s_texture);
}

void RocketLauncherPowerUp::apply(Player &player)
{
    PowerUp::apply(player);

    m_timeToLive = ROCKET_LAUNCHER_POWERUP_DURATION;
    player.switchGun(GunType::ROCKET_LAUNCHER);
}

void RocketLauncherPowerUp::remove(Player &player)
{
    player.switchGun(GunType::GUN);
}