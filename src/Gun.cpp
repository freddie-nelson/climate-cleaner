#include "../include/Gun.h"

#include <remi/Rendering/Mesh/Mesh.h>
#include <remi/Rendering/Renderable.h>
#include <remi/Rendering/Material/Material.h>
#include <remi/Core/Transform.h>
#include <remi/Physics/Collider2D.h>
#include <remi/Config.h>
#include <glm/glm.hpp>

Gun::Gun(remi::Engine &engine, ECS::Entity holder, float holderHeight, GunType gunType)
    : m_engine(engine), m_holder(holder), m_holderHeight(holderHeight), m_gunType(gunType)
{
    makeGun();
    makeCrosshair();

    auto &world = *m_engine.getWorld();
    world.addSystem(this);
}

Gun::~Gun()
{
    destroyGun();
    destroyCrosshair();

    auto &world = *m_engine.getWorld();
    world.removeSystem(this);
}

void Gun::update(World::World &world, const Core::Timestep &timestep)
{
    updateCrosshair();
}

void Gun::fixedUpdate(World::World &world, const Core::Timestep &timestep)
{
    updateGun(timestep);
}

void Gun::switchGun(GunType gunType)
{
    m_gunType = gunType;

    destroyGun();
    makeGun();
}

void Gun::makeGun()
{
    auto &world = *m_engine.getWorld();
    auto &registry = world.getRegistry();
    auto &sceneGraph = world.getSceneGraph();

    if (m_gunExists)
    {
        destroyGun();
    }

    m_gunExists = true;

    m_gun = registry.create();
    registry.add(m_gun, Core::Transform());
    registry.add(m_gun, Rendering::Mesh2D(GUN_WIDTH, GUN_HEIGHT));
    registry.add(m_gun, Rendering::Material(Rendering::Color(0.5f, 0.5f, 0.5f, 1.0f), getGunTexture()));
    registry.add(m_gun, Rendering::Renderable(true, false));

    sceneGraph.relate(m_holder, m_gun);
}

void Gun::destroyGun()
{
    auto &world = *m_engine.getWorld();
    auto &registry = world.getRegistry();

    registry.destroy(m_gun);
}

void Gun::updateGun(const Core::Timestep &timestep)
{
    m_fireTimer += timestep.getSeconds();

    auto &world = *m_engine.getWorld();
    auto &registry = world.getRegistry();
    auto &sceneGraph = world.getSceneGraph();
    auto &mouse = *m_engine.getMouse();

    auto holderToMouse = getHolderToMouse(world);
    auto holderToMouseDir = glm::normalize(holderToMouse);

    auto &gunTransform = registry.get<Core::Transform>(m_gun);

    // flip in direction of fire
    auto normal = glm::vec2(-holderToMouseDir.y, holderToMouseDir.x);
    auto barrelOffset = GUN_BARREL_OFFSET * normal;

    if (glm::dot(holderToMouseDir, glm::vec2(1, 0)) >= 0)
    {
        // facing right
        gunTransform.setScale(glm::vec2(1.0f, 1.0f));
        gunTransform.setRotation(glm::atan(holderToMouseDir.y, holderToMouseDir.x));
    }
    else
    {
        gunTransform.setScale(glm::vec2(1.0f, -1.0f));
        gunTransform.setRotation(-glm::atan(holderToMouseDir.y, holderToMouseDir.x));
        barrelOffset *= -1.2f;
    }

    gunTransform.setTranslation(glm::vec2(holderToMouseDir * m_holderHeight * 0.65f));

    if (mouse.isPressed(Input::MouseButton::LEFT) && m_fireTimer >= GUN_FIRE_RATE)
    {
        auto totalSpread = (GUN_BULLETS - 1) * GUN_BULLET_SPREAD;

        for (size_t i = 0; i < GUN_BULLETS; i++)
        {
            auto offset = (normal * (i * GUN_BULLET_SPREAD)) - (normal * totalSpread / 2.0f);
            auto bullet = new Bullet(m_engine, Core::Transform(sceneGraph.getModelMatrix(m_gun)).getTranslation() + offset + barrelOffset, holderToMouseDir, BULLET_SPEED, BULLET_LIFETIME, m_gunType == GUN ? BULLET : m_gunType == FREEZE_GUN ? FREEZE_BULLET
                                                                                                                                                                                                                                                : EXPLODING_BULLET);
        }

        m_fireTimer = 0.0f;
    }
}

void Gun::makeCrosshair()
{
    if (m_crosshairExists)
    {
        destroyCrosshair();
    }

    auto &world = *m_engine.getWorld();
    auto &registry = world.getRegistry();
    auto &sceneGraph = world.getSceneGraph();

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

void Gun::destroyCrosshair()
{
    auto &world = *m_engine.getWorld();
    auto &registry = world.getRegistry();

    auto &children = world.getSceneGraph().children(m_crosshair);
    for (auto child : children)
    {
        registry.destroy(child);
    }

    registry.destroy(m_crosshair);
}

void Gun::updateCrosshair()
{
    auto &world = *m_engine.getWorld();
    auto &registry = world.getRegistry();
    auto &sceneGraph = world.getSceneGraph();

    auto worldMouse = getWorldMousePosition(world);

    auto &crosshairTransform = registry.get<Core::Transform>(m_crosshair);
    crosshairTransform.setTranslation(worldMouse);
}

glm::vec2 Gun::getHolderToMouse(World::World &world)
{
    auto &registry = world.getRegistry();
    auto &sceneGraph = world.getSceneGraph();

    auto worldMousePos = getWorldMousePosition(world);

    auto playerWorldTransform = Core::Transform(sceneGraph.getModelMatrix(m_holder));
    auto playerWorldPos = playerWorldTransform.getTranslation();

    auto playerToMouse = worldMousePos - playerWorldPos;

    return playerToMouse;
}

glm::vec2 Gun::getWorldMousePosition(World::World &world)
{
    auto &registry = world.getRegistry();
    auto &sceneGraph = world.getSceneGraph();

    auto &spaceTransformer = *m_engine.getSpaceTransformer();
    auto &mouse = *m_engine.getMouse();

    auto screenMousePos = mouse.getPosition(true);
    auto worldMousePos = spaceTransformer.transform(screenMousePos, Core::SpaceTransformer::Space::SCREEN, Core::SpaceTransformer::Space::WORLD);

    return worldMousePos;
}

Rendering::Texture *Gun::getGunTexture()
{
    switch (m_gunType)
    {
    case GunType::GUN:
        return &m_gunTexture;
    default:
        throw std::runtime_error("gun type not implemented");
    }
}