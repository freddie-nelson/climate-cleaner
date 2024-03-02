#pragma once

#include "PowerUp.h"

#include <remi/Engine.h>

#define HEALTH_POWERUP_HEAL_AMOUNT 15

class HealthPowerUp : public PowerUp
{
public:
    HealthPowerUp(remi::Engine &engine, glm::vec2 position, float timeToLive);

    void apply(Player &player) override;
    void remove(Player &player) override;

    const std::vector<PowerUpType> &getIncompatiblePowerUps() const override
    {
        static std::vector<PowerUpType> incompatiblePowerUps = {PowerUpType::HEALTH_POWERUP};
        return incompatiblePowerUps;
    }

private:
    static Rendering::Texture *s_texture;
};