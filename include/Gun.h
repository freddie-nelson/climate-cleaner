#pragma once

#include "Bullet.h"

#include <remi/Engine.h>
#include <remi/Rendering/Texture/Texture.h>

#define GUN_POSITION glm::vec2(PLAYER_WIDTH / 2.0f, 0.0f);
#define GUN_FIRE_RATE 0.15f
#define GUN_BULLETS 1
#define GUN_BULLET_SPREAD 0.2f
#define GUN_BARREL_OFFSET 0.1f

#define CROSSHAIR_SIZE 0.08f
#define CROSSHAIR_GAP 0.1f
#define CROSSHAIR_COLOR Rendering::Color(0.5f, 0.5f, 0.5f, 1.0f)

#define BULLET_LIFETIME 3.0f
#define BULLET_SPEED 15.0f

enum GunType
{
    GUN,
    FREEZE_GUN,
    ROCKET_LAUNCHER,
};

class Gun : public World::System
{
public:
    /**
     * Create a new gun
     *
     * @param engine The engine
     * @param holder The entity that holds the gun
     * @param holderHeight The height of the holder
     * @param gunType The type of gun
     */
    Gun(remi::Engine &engine, ECS::Entity holder, float holderHeight, GunType gunType = GUN);

    ~Gun();

    void update(World::World &world, const Core::Timestep &timestep) override;

    void fixedUpdate(World::World &world, const Core::Timestep &timestep) override;

    void switchGun(GunType gunType);

private:
    remi::Engine &m_engine;
    ECS::Entity m_holder;
    float m_holderHeight;

    GunType m_gunType;

    bool m_gunExists = false;
    ECS::Entity m_gun;
    float m_fireTimer = 0.0f;

    bool m_crosshairExists = false;
    ECS::Entity m_crosshair;

    void makeGun();
    void destroyGun();
    void updateGun(const Core::Timestep &timestep);

    void makeCrosshair();
    void destroyCrosshair();
    void updateCrosshair();

    glm::vec2 getHolderToMouse(World::World &world);
    glm::vec2 getWorldMousePosition(World::World &world);

    Rendering::Texture m_gunTexture = Rendering::Texture("assets/images/gun.png");
    Rendering::Texture m_freezeTexture = Rendering::Texture("assets/images/freeze-gun.png");
    Rendering::Texture m_rocketTexture = Rendering::Texture("assets/images/rocket-launcher.png");

    Rendering::Texture *getGunTexture();

    float getWidth();
    float getHeight();
};