#pragma once

#include <remi/Engine.h>
#include <remi/Physics/RigidBody2D.h>

#define BULLET_WIDTH 0.2f
#define BULLET_HEIGHT 0.1f

struct BulletTag
{
};

enum BulletType
{
    BULLET,
    FREEZE_BULLET,
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

    Physics::RigidBodyCollisionCallback m_hitCallback = nullptr;

    void callback(const Physics::ContactInfo &contactInfo);
};