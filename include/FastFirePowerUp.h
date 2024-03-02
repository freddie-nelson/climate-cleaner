#pragma once

#include "PowerUp.h"

#include <remi/Engine.h>

#define FAST_FIRE_POWER_UP_DURATION 15.0f

class FastFirePowerUp : public PowerUp
{
public:
    FastFirePowerUp(remi::Engine &engine, glm::vec2 position, float timeToLive);

    void apply(Player &player) override;
    void remove(Player &player) override;

    const std::vector<PowerUpType> &getIncompatiblePowerUps() const override
    {
        static std::vector<PowerUpType> incompatiblePowerUps = {PowerUpType::FAST_FIRE_POWERUP};
        return incompatiblePowerUps;
    }

private:
    static Rendering::Texture *s_texture;
};