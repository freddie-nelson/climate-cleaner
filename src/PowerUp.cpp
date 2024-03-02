#include "../include/PowerUp.h"
#include "../include/Layers.h"

#include <remi/Rendering/Material/Material.h>
#include <remi/Rendering/Mesh/Mesh.h>
#include <remi/Rendering/Renderable.h>

PowerUp::PowerUp(remi::Engine &engine, glm::vec2 position, PowerUpType type, float timeToLive)
    : m_engine(engine), m_position(position), m_type(type), m_timeToLive(timeToLive)
{
    auto &world = *engine.getWorld();
    auto &registry = world.getRegistry();
    auto &sceneGraph = world.getSceneGraph();

    m_entity = registry.create();
    auto &t = registry.add(m_entity, Core::Transform(position));
    t.setZIndex(POWERUP_LAYER);

    registry.add(m_entity, Rendering::Mesh2D(POWERUP_SIZE, POWERUP_SIZE));
    registry.add(m_entity, Rendering::Material(POWERUP_COLOR));
    registry.add(m_entity, Rendering::Renderable(true, true));
    registry.add(m_entity, HealthBarTag{timeToLive, timeToLive});

    m_healthBar = new HealthBar(engine, m_entity, glm::vec2(0, POWERUP_SIZE * 0.6f), glm::vec2(1.0f, 1.0f), true, Rendering::Color(0.0f, 0.0f, 0.0f, 1.0f), Rendering::Color(1.0f, 1.0f, 1.0f, 1.0f));
}

PowerUpType PowerUp::getType() const
{
    return m_type;
}

float PowerUp::getDuration() const
{
    return m_timeToLive;
}

const glm::vec2 &PowerUp::getPosition() const
{
    return m_position;
}

void PowerUp::update(World::World &world, const Core::Timestep &timestep)
{
    m_timeToLive -= timestep.getSeconds();

    if (isDestroyed())
        return;

    auto &registry = world.getRegistry();
    auto &healthBarTag = registry.get<HealthBarTag>(m_entity);
    healthBarTag.health = m_timeToLive;

    if (isExpired())
    {
        destroy();
    }
}

void PowerUp::destroy()
{
    if (isDestroyed())
        return;

    m_timeToLive = 0;
    auto &world = *m_engine.getWorld();
    auto &registry = world.getRegistry();

    registry.destroy(m_entity);
    m_destroyed = true;
}

void PowerUp::apply(Player &player)
{
    m_pickedUp = true;
}

bool PowerUp::isExpired() const
{
    return m_timeToLive <= 0.0f;
}

bool PowerUp::isPickedUp() const
{
    return m_pickedUp;
}

bool PowerUp::isDestroyed() const
{
    return m_destroyed;
}