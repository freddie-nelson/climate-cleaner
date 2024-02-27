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

    m_gunType = ROCKET_LAUNCHER;
    makeGun();

    world.addSystem(this);
}

void Player::update(World::World &world, const Core::Timestep &timestep)
{
    handleCrosshair(world);
}

void Player::fixedUpdate(World::World &world, const Core::Timestep &timestep)
{
    handleMovement(world, timestep);
    handleGun(world, timestep);
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
    m_gunType = gunType;
    makeGun();
}

void Player::makeGun()
{
    auto &world = *m_engine.getWorld();
    auto &registry = world.getRegistry();
    auto &sceneGraph = world.getSceneGraph();

    if (hasGun)
    {
        registry.destroy(m_gun);

        auto &children = sceneGraph.children(m_crosshair);
        for (auto child : children)
        {
            registry.destroy(child);
        }

        registry.destroy(m_crosshair);
    }

    if (m_gunType != NONE)
    {
        m_gun = registry.create();
        registry.add(m_gun, Core::Transform());
        registry.add(m_gun, Rendering::Mesh2D(GUN_WIDTH, GUN_HEIGHT));
        registry.add(m_gun, Rendering::Material(Rendering::Color(0.5f, 0.5f, 0.5f, 1.0f)));
        registry.add(m_gun, Rendering::Renderable(true, false));

        sceneGraph.relate(m_player, m_gun);

        hasGun = true;
    }
    else
    {
        hasGun = false;
    }

    m_crosshair = registry.create();
    auto &t = registry.add(m_crosshair, Core::Transform());

    // left
    auto left = registry.create();
    auto &leftT = registry.add(left, Core::Transform(glm::vec2(-CROSSHAIR_GAP, 0.0f)));
    registry.add(left, Rendering::Mesh2D(CROSSHAIR_SIZE, CROSSHAIR_SIZE / 2.0f));
    registry.add(left, Rendering::Material(CROSSHAIR_COLOR));
    registry.add(left, Rendering::Renderable(true, false));

    leftT.setZIndex(Config::MAX_Z_INDEX);
    sceneGraph.relate(m_crosshair, left);

    // right
    auto right = registry.create();
    auto &rightT = registry.add(right, Core::Transform(glm::vec2(CROSSHAIR_GAP, 0.0f)));
    registry.add(right, Rendering::Mesh2D(CROSSHAIR_SIZE, CROSSHAIR_SIZE / 2.0f));
    registry.add(right, Rendering::Material(CROSSHAIR_COLOR));
    registry.add(right, Rendering::Renderable(true, false));

    rightT.setZIndex(Config::MAX_Z_INDEX);
    sceneGraph.relate(m_crosshair, right);

    // up
    auto up = registry.create();
    auto &upT = registry.add(up, Core::Transform(glm::vec2(0.0f, CROSSHAIR_GAP)));
    registry.add(up, Rendering::Mesh2D(CROSSHAIR_SIZE / 2.0f, CROSSHAIR_SIZE));
    registry.add(up, Rendering::Material(CROSSHAIR_COLOR));
    registry.add(up, Rendering::Renderable(true, false));

    upT.setZIndex(Config::MAX_Z_INDEX);
    sceneGraph.relate(m_crosshair, up);

    // down
    auto down = registry.create();
    auto &downT = registry.add(down, Core::Transform(glm::vec2(0.0f, -CROSSHAIR_GAP)));
    registry.add(down, Rendering::Mesh2D(CROSSHAIR_SIZE / 2.0f, CROSSHAIR_SIZE));
    registry.add(down, Rendering::Material(CROSSHAIR_COLOR));
    registry.add(down, Rendering::Renderable(true, false));

    downT.setZIndex(Config::MAX_Z_INDEX);
    sceneGraph.relate(m_crosshair, down);
}

void Player::handleGun(World::World &world, const Core::Timestep &timestep)
{
    if (!hasGun)
    {
        return;
    }

    m_fireTimer += timestep.getSeconds();

    auto &registry = world.getRegistry();
    auto &sceneGraph = world.getSceneGraph();
    auto &mouse = *m_engine.getMouse();

    auto playerToMouse = getPlayerToMouse(world);
    auto playerToMouseDir = glm::normalize(playerToMouse);

    auto &gunTransform = registry.get<Core::Transform>(m_gun);

    gunTransform.setTranslation(glm::vec2(playerToMouseDir * PLAYER_HEIGHT * 0.75f));
    gunTransform.setRotation(glm::atan(playerToMouseDir.y, playerToMouseDir.x));

    if (mouse.isPressed(Input::MouseButton::LEFT) && m_fireTimer >= GUN_FIRE_RATE)
    {
        auto normal = glm::vec2(-playerToMouseDir.y, playerToMouseDir.x);
        auto totalSpread = (GUN_BULLETS - 1) * GUN_BULLET_SPREAD;

        for (size_t i = 0; i < GUN_BULLETS; i++)
        {
            auto offset = (normal * (i * GUN_BULLET_SPREAD)) - (normal * totalSpread / 2.0f);
            auto bullet = new Bullet(m_engine, Core::Transform(sceneGraph.getModelMatrix(m_gun)).getTranslation() + offset, playerToMouseDir, PLAYER_BULLET_SPEED, PLAYER_BULLET_LIFETIME, m_gunType == GUN ? BULLET : m_gunType == FREEZE_GUN ? FREEZE_BULLET
                                                                                                                                                                                                                                               : EXPLODING_BULLET);
        }

        m_fireTimer = 0.0f;
    }
}

void Player::handleCrosshair(World::World &world)
{
    auto &registry = world.getRegistry();
    auto &sceneGraph = world.getSceneGraph();

    auto worldMouse = getWorldMousePosition(world);

    auto &crosshairTransform = registry.get<Core::Transform>(m_crosshair);
    crosshairTransform.setTranslation(worldMouse);
}

glm::vec2 Player::getPlayerToMouse(World::World &world)
{
    auto &registry = world.getRegistry();
    auto &sceneGraph = world.getSceneGraph();

    auto worldMousePos = getWorldMousePosition(world);

    auto playerWorldTransform = Core::Transform(sceneGraph.getModelMatrix(m_player));
    auto playerWorldPos = playerWorldTransform.getTranslation();

    auto playerToMouse = worldMousePos - playerWorldPos;

    return playerToMouse;
}

glm::vec2 Player::getWorldMousePosition(World::World &world)
{
    auto &registry = world.getRegistry();
    auto &sceneGraph = world.getSceneGraph();

    auto &spaceTransformer = *m_engine.getSpaceTransformer();
    auto &mouse = *m_engine.getMouse();

    auto screenMousePos = mouse.getPosition(true);
    auto worldMousePos = spaceTransformer.transform(screenMousePos, Core::SpaceTransformer::Space::SCREEN, Core::SpaceTransformer::Space::WORLD);

    return worldMousePos;
}
