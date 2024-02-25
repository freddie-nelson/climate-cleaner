#pragma once

#include "Bullet.h"

#include <remi/Engine.h>
#include <glm/glm.hpp>

#define PLAYER_WIDTH 0.5f
#define PLAYER_HEIGHT 1.0f
#define MAX_PLAYER_SPEED 2.0f
#define MAX_PLAYER_SPRINT_SPEED 4.0f
#define PLAYER_MOVE_FORCE 10.0f
#define PLAYER_FRICITON 0.75f
#define PLAYER_JUMP_FORCE 50.0f

#define GUN_WIDTH 0.1f
#define GUN_HEIGHT 0.1f
#define GUN_POSITION glm::vec2(PLAYER_WIDTH / 2.0f, 0.0f);

#define PLAYER_BULLET_LIFETIME 3.0f
#define PLAYER_BULLET_SPEED 10.0f

enum GunType
{
    NONE,
    GUN,
    FREEZE_GUN,
};

class Player : public World::System
{
public:
    Player(remi::Engine &engine, glm::vec2 position = glm::vec2(0));

    void update(World::World &world, const Core::Timestep &timestep) override;

    void fixedUpdate(World::World &world, const Core::Timestep &timestep) override;

    void switchGun(GunType gunType);

private:
    remi::Engine &m_engine;

    ECS::Entity m_player;
    ECS::Entity m_sprite;
    ECS::Entity m_camera;

    void handleMovement(World::World &world, const Core::Timestep &timestep);

    bool hasGun = false;
    GunType m_gunType = NONE;
    ECS::Entity m_gun;
    ECS::Entity m_crosshair;

    void makeGun();
    void handleGun(World::World &world, const Core::Timestep &timestep);

    void handleCrosshair(World::World &world);

    glm::vec2 getPlayerToMouse(World::World &world);

    glm::vec2 getWorldMousePosition(World::World &world);
};