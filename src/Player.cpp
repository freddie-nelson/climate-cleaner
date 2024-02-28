#include "../include/Player.h"

#include <remi/Rendering/Camera/Camera.h>
#include <remi/Rendering/Camera/ActiveCamera.h>
#include <remi/Rendering/Mesh/Mesh.h>
#include <remi/Rendering/Material/Material.h>
#include <remi/Rendering/Renderable.h>
#include <remi/Core/Transform.h>
#include <remi/Physics/RigidBody2D.h>
#include <remi/Physics/Collider2D.h>

Player::Player(remi::Engine &engine, glm::vec2 position) : m_engine(engine)
{
    auto &world = *engine.getWorld();
    auto &registry = world.getRegistry();
    auto &sceneGraph = world.getSceneGraph();

    // create player
    m_player = registry.create();
    registry.add(m_player, PlayerTag{this});
    registry.add(m_player, Core::Transform(position));

    auto &body = registry.add(m_player, Physics::RigidBody2D());
    body.setFixedRotation(true);
    body.setLinearDamping(PLAYER_DAMPING);

    auto playerColliderShape = Physics::PolygonColliderShape2D(Rendering::Mesh2D(PLAYER_WIDTH, PLAYER_HEIGHT));
    auto &collider = registry.add(m_player, Physics::Collider2D(&playerColliderShape));
    collider.setFriction(PLAYER_FRICITON);

    // create player sprite
    m_sprite = registry.create();
    registry.add(m_sprite, Core::Transform());
    registry.add(m_sprite, Rendering::Mesh2D(PLAYER_WIDTH, PLAYER_HEIGHT));
    registry.add(m_sprite, Rendering::Material());
    registry.add(m_sprite, Rendering::Renderable(true, false));

    // create camera
    m_camera = registry.create();
    registry.add(m_camera, Core::Transform());
    registry.add(m_camera, Rendering::Camera(engine.getRenderer()->getSize().x, engine.getRenderer()->getSize().y));
    registry.add(m_camera, Rendering::ActiveCamera());

    // child camera and sprite to player
    sceneGraph.relate(m_player, m_camera);
    sceneGraph.relate(m_player, m_sprite);

    world.addSystem(this);

    switchGun(GunType::GUN);
}

void Player::fixedUpdate(World::World &world, const Core::Timestep &timestep)
{
    handleMovement(world, timestep);

    // material
    auto &registry = world.getRegistry();
    auto &material = registry.get<Rendering::Material>(m_sprite);

    if (m_hitTimer > 0)
    {
        m_hitTimer -= timestep.getSeconds();

        material.setColor(PLAYER_HIT_COLOR);
    }
    else
    {
        material.setColor(PLAYER_COLOR);
    }
}

void Player::takeDamage(float damage)
{
    m_health -= damage;
    m_hitTimer = PLAYER_HIT_DURATION;

    if (m_health <= 0)
    {
        std::cout << "Player died" << std::endl;
    }
}

void Player::handleMovement(World::World &world, const Core::Timestep &timestep)
{
    auto &spaceTransformer = *m_engine.getSpaceTransformer();
    auto &physicsWorld = *m_engine.getPhysicsWorld();

    auto &registry = world.getRegistry();
    auto &sceneGraph = world.getSceneGraph();

    auto &playerTransform = registry.get<Core::Transform>(m_player);
    auto &playerBody = registry.get<Physics::RigidBody2D>(m_player);

    auto &spriteTransform = registry.get<Core::Transform>(m_sprite);

    auto &keyboard = *m_engine.getKeyboard();

    // apply force to player
    if (keyboard.isPressed(Input::Key::A))
    {
        playerBody.applyForce(glm::vec2(-PLAYER_MOVE_FORCE, 0.0f));
    }
    if (keyboard.isPressed(Input::Key::D))
    {
        playerBody.applyForce(glm::vec2(PLAYER_MOVE_FORCE, 0.0f));
    }

    if (keyboard.isPressed(Input::Key::W))
    {
        playerBody.applyForce(glm::vec2(0.0f, PLAYER_MOVE_FORCE));
    }
    if (keyboard.isPressed(Input::Key::S))
    {
        playerBody.applyForce(glm::vec2(0.0f, -PLAYER_MOVE_FORCE));
    }

    // clamp player speed
    auto velocity = playerBody.getVelocity();
    float maxSpeed = keyboard.isPressed(Input::Key::LEFT_SHIFT) ? MAX_PLAYER_SPRINT_SPEED : MAX_PLAYER_SPEED;

    if (glm::length(velocity) > maxSpeed)
    {
        velocity = glm::normalize(velocity) * maxSpeed;
    }

    playerBody.setVelocity(velocity);

    // jump

    // check if player is grounded
    auto origin = spaceTransformer.transform(playerTransform.getTranslation(), m_player, Core::SpaceTransformer::Space::LOCAL, Core::SpaceTransformer::Space::WORLD);
    auto direction = glm::vec2(0, -1);
    auto length = PLAYER_HEIGHT / 2.0f + 0.1f;
    Physics::Ray ray(origin, direction, length);

    auto results = physicsWorld.raycast(ray, Physics::RaycastType::ANY);

    // player is grounded so can jump
    if (results.size() > 0)
    {
        if (keyboard.isPressed(Input::Key::SPACE))
        {
            playerBody.applyForce(glm::vec2(0.0f, PLAYER_JUMP_FORCE));
        }
    }
}

void Player::switchGun(GunType gunType)
{
    if (m_gun == nullptr)
    {
        m_gun = new Gun(m_engine, m_player, PLAYER_HEIGHT, gunType);
    }
    else
    {
        m_gun->switchGun(gunType);
    }
}
