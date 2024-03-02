#include "../include/FreezeGunPowerUp.h"

#include <iostream>

Rendering::Texture *FreezeGunPowerUp::s_texture = new Rendering::Texture("assets/images/freeze-gun.png");

FreezeGunPowerUp::FreezeGunPowerUp(remi::Engine &engine, glm::vec2 position, float timeToLive)
    : PowerUp(engine, position, PowerUpType::FREEZE_GUN_POWERUP, timeToLive)
{
    auto &registry = m_engine.getWorld()->getRegistry();
    auto &material = registry.get<Rendering::Material>(m_entity);
    material.setTexture(s_texture);
}

void FreezeGunPowerUp::apply(Player &player)
{
    PowerUp::apply(player);

    m_timeToLive = ROCKET_LAUNCHER_POWERUP_DURATION;
    player.switchGun(GunType::FREEZE_GUN);
}

void FreezeGunPowerUp::remove(Player &player)
{
    player.switchGun(GunType::GUN);
}