#pragma once

#include <remi/Engine.h>
#include "Player.h"

#define POWERUP_SIZE 1.0f
#define POWERUP_PICKUP_RADIUS 1.0f
#define POWERUP_COLOR Rendering::Color(1.0f, 1.0f, 1.0f, 1.0f)

enum PowerUpType
{
    HEALTH_POWERUP,
    FREEZE_GUN_POWERUP,
    ROCKET_LAUNCHER_POWERUP,
    DOUBLE_BARREL_POWERUP,
    FAST_FIRE_POWERUP,
};

class PowerUp
{
public:
    /**
     * Create a new power up.
     */
    PowerUp(remi::Engine &engine, glm::vec2 position, PowerUpType type, float timeToLive);

    virtual ~PowerUp() = default;

    PowerUpType getType() const;
    float getDuration() const;
    const glm::vec2 &getPosition() const;

    virtual void update(World::World &world, const Core::Timestep &timestep);

    virtual void destroy();

    virtual void apply(Player &player);
    virtual void remove(Player &player) = 0;

    bool isExpired() const;

    bool isPickedUp() const;

    bool isDestroyed() const;

    virtual const std::vector<PowerUpType> &getIncompatiblePowerUps() const = 0;

protected:
    remi::Engine &m_engine;
    glm::vec2 m_position;
    PowerUpType m_type;
    float m_timeToLive;
    bool m_pickedUp = false;
    bool m_destroyed = false;

    ECS::Entity m_entity;
    HealthBar *m_healthBar = nullptr;
};
