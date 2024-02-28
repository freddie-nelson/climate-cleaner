#pragma once

#include "Gun.h"

#include <remi/Engine.h>
#include <glm/glm.hpp>

#define PLAYER_WIDTH 0.5f
#define PLAYER_HEIGHT 1.0f
#define MAX_PLAYER_SPEED 2.0f
#define MAX_PLAYER_SPRINT_SPEED 4.0f
#define PLAYER_MOVE_FORCE 20.0f
#define PLAYER_FRICITON 0.75f
#define PLAYER_JUMP_FORCE 50.0f
#define PLAYER_DAMPING 5.0f
#define PLAYER_HEALTH 100.0f
#define PLAYER_COLOR Rendering::Color(1.0f, 1.0f, 1.0f, 1.0f)
#define PLAYER_HIT_COLOR Rendering::Color(1.0f, 0.0f, 0.0f, 1.0f)
#define PLAYER_HIT_DURATION 0.1f

class Player : public World::System
{
public:
    Player(remi::Engine &engine, glm::vec2 position = glm::vec2(0));

    void fixedUpdate(World::World &world, const Core::Timestep &timestep) override;

    void switchGun(GunType gunType);

    ECS::Entity getEntity() const { return m_player; }

    void takeDamage(float damage);

private:
    remi::Engine &m_engine;

    float m_hitTimer = 0.0f;
    float m_health = PLAYER_HEALTH;

    ECS::Entity m_player;
    ECS::Entity m_sprite;
    ECS::Entity m_camera;

    void handleMovement(World::World &world, const Core::Timestep &timestep);

    bool hasGun = false;
    Gun *m_gun = nullptr;
};

struct PlayerTag
{
    Player *player;
};