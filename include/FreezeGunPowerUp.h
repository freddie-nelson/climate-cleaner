#pragma once

#include "PowerUp.h"

#include <remi/Engine.h>

#define ROCKET_LAUNCHER_POWERUP_DURATION 15.0f

class FreezeGunPowerUp : public PowerUp
{
public:
    FreezeGunPowerUp(remi::Engine &engine, glm::vec2 position, float timeToLive);

    void apply(Player &player) override;
    void remove(Player &player) override;

    const std::vector<PowerUpType> &getIncompatiblePowerUps() const override
    {
        static std::vector<PowerUpType> incompatiblePowerUps = {PowerUpType::FREEZE_GUN_POWERUP, PowerUpType::ROCKET_LAUNCHER_POWERUP};
        return incompatiblePowerUps;
    }

private:
    static Rendering::Texture *s_texture;
};