#pragma once

#include "PowerUp.h"

#include <remi/Engine.h>

#define DOUBLE_BARREL_POWERUP_DURATION 15.0f

class DoubleBarrelPowerUp : public PowerUp
{
public:
    DoubleBarrelPowerUp(remi::Engine &engine, glm::vec2 position, float timeToLive);

    void apply(Player &player) override;
    void remove(Player &player) override;

    const std::vector<PowerUpType> &getIncompatiblePowerUps() const override
    {
        static std::vector<PowerUpType> incompatiblePowerUps = {PowerUpType::DOUBLE_BARREL_POWERUP};
        return incompatiblePowerUps;
    }

private:
    static Rendering::Texture *s_texture;
};