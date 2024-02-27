#pragma once

#include <remi/Engine.h>
#include <remi/Physics/RigidBody2D.h>

#define BULLET_WIDTH 0.2f
#define BULLET_HEIGHT 0.1f

#define EXPLODING_BULLET_COLOR Rendering::Color(1.0f, 0.2f, 0.0f, 0.5f)
#define EXPLODING_BULLET_RADIUS 1.5f
#define EXPLODING_BULLET_SPEED 1.2f
#define EXPLODING_BULLET_STARTING_RADIUS 0.15f
#define EXPLODING_BULLET_FORCE 200.0f

struct BulletTag
{
};

enum BulletType
{
    BULLET,
    FREEZE_BULLET,
    EXPLODING_BULLET,
};

class Bullet : public World::System
{
public:
    Bullet(remi::Engine &engine, glm::vec2 position, glm::vec2 direction, float speed, float lifeTime, BulletType bulletType = BULLET);

    void setHitCallback(Physics::RigidBodyCollisionCallback callback);

    void fixedUpdate(World::World &world, const Core::Timestep &timestep);

private:
    remi::Engine &m_engine;

    ECS::Entity m_entity;
    float m_lifeTime;
    BulletType m_bulletType;
    glm::vec2 m_direction;
    float m_speed;

    bool m_exploding = false;
    bool m_createdExplosion = false;
    ECS::Entity m_explosion;

    Physics::RigidBodyCollisionCallback m_hitCallback = nullptr;

    void callback(const Physics::ContactInfo &contactInfo);
};