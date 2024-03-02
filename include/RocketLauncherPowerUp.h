#pragma once

#include "PowerUp.h"

#include <remi/Engine.h>

#define ROCKET_LAUNCHER_POWERUP_DURATION 15.0f

class RocketLauncherPowerUp : public PowerUp
{
public:
    RocketLauncherPowerUp(remi::Engine &engine, glm::vec2 position, float timeToLive);

    void apply(Player &player) override;
    void remove(Player &player) override;

    const std::vector<PowerUpType> &getIncompatiblePowerUps() const override
    {
        static std::vector<PowerUpType> incompatiblePowerUps = {PowerUpType::ROCKET_LAUNCHER_POWERUP, PowerUpType::FREEZE_GUN_POWERUP};
        return incompatiblePowerUps;
    }

private:
    static Rendering::Texture *s_texture;
};