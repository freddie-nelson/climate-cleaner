#include "../include/HealthBar.h"
#include "../include/Layers.h"

#include <math.h>
#include <remi/Rendering/Renderable.h>
#include <remi/Rendering/Mesh/Mesh.h>
#include <remi/Rendering/Material/Material.h>
#include <remi/Core/Transform.h>

HealthBar::HealthBar(remi::Engine &engine, ECS::Entity owner, glm::vec2 position, glm::vec2 scale, bool alwaysVisible, Rendering::Color backgroundColor, Rendering::Color barColor) : m_engine(engine), m_owner(owner), m_alwaysVisible(alwaysVisible)
{
    auto &world = *m_engine.getWorld();
    auto &registry = world.getRegistry();
    auto &sceneGraph = world.getSceneGraph();

    m_bar = registry.create();

    auto &t = registry.add(m_bar, Core::Transform());
    t.setZIndex(HEALTHBAR_LAYER + 1);

    registry.add(m_bar, Rendering::Mesh2D(HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT));
    registry.add(m_bar, Rendering::Material(barColor));
    registry.add(m_bar, Rendering::Renderable(true, false));

    m_container = registry.create();

    auto &containerT = registry.add(m_container, Core::Transform(position));
    containerT.setZIndex(HEALTHBAR_LAYER);
    containerT.setScale(scale);

    registry.add(m_container, Rendering::Mesh2D(HEALTH_BAR_WIDTH + HEALTH_BAR_BORDER_SIZE * 2, HEALTH_BAR_HEIGHT + HEALTH_BAR_BORDER_SIZE * 2));
    registry.add(m_container, Rendering::Material(backgroundColor));
    registry.add(m_container, Rendering::Renderable(true, false));

    sceneGraph.relate(m_owner, m_container);
    sceneGraph.relate(m_container, m_bar);

    updateVisibility();

    world.addSystem(this);
}

void HealthBar::fixedUpdate(World::World &world, const Core::Timestep &timestep)
{
    auto &registry = world.getRegistry();

    if (!registry.has<HealthBarTag>(m_owner))
    {
        registry.destroy(m_bar);
        registry.destroy(m_container);
        world.removeSystem(this);
        delete this;

        return;
    }

    auto &healthBarTag = registry.get<HealthBarTag>(m_owner);
    float healthPercentage = std::fmax(0, healthBarTag.health / healthBarTag.maxHealth);

    auto &t = registry.get<Core::Transform>(m_bar);
    t.setScale(glm::vec2(healthPercentage, 1.0f));
    t.setTranslation(glm::vec2(-HEALTH_BAR_WIDTH / 2 * (1 - healthPercentage), 0.0f));

    updateVisibility();
}

void HealthBar::updateVisibility()
{
    auto &world = *m_engine.getWorld();
    auto &registry = world.getRegistry();

    auto &containerRenderable = registry.get<Rendering::Renderable>(m_container);
    auto &barRenderable = registry.get<Rendering::Renderable>(m_bar);

    if (m_alwaysVisible)
    {
        containerRenderable.isVisible = true;
        barRenderable.isVisible = true;
        return;
    }

    auto &healthBarTag = registry.get<HealthBarTag>(m_owner);
    if (healthBarTag.health < healthBarTag.maxHealth)
    {
        containerRenderable.isVisible = true;
        barRenderable.isVisible = true;
    }
    else
    {
        containerRenderable.isVisible = false;
        barRenderable.isVisible = false;
    }
}