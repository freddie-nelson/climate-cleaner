#include "../include/DeathScreen.h"
#include "../include/Layers.h"

#include <remi/Rendering/Mesh/Mesh.h>
#include <remi/Rendering/Material/Material.h>
#include <remi/Core/Transform.h>
#include <remi/Rendering/Renderable.h>

DeathScreen::DeathScreen(remi::Engine &engine, Player &player, EnemySpawner &spawner)
    : m_engine(engine), m_player(player), m_spawner(spawner)
{
    auto &world = *m_engine.getWorld();
    world.addSystem(this);
}

void DeathScreen::update(World::World &world, const Core::Timestep &timestep)
{
    if (m_player.isDead() && !m_dead)
    {
        createDeathScreen();
        m_dead = true;
    }

    if (m_dead)
    {
        updateDeathScreen(timestep);
    }
}

void DeathScreen::createDeathScreen()
{
    m_fadeInTimer = 0.0f;

    auto &world = *m_engine.getWorld();
    auto &registry = world.getRegistry();
    auto &renderer = *m_engine.getRenderer();
    auto &spaceTransformer = *m_engine.getSpaceTransformer();

    m_deathScreen = registry.create();
    auto &t = registry.add(m_deathScreen, Core::Transform());
    t.setZIndex(DEATH_SCREEN_LAYER);

    registry.add(m_deathScreen, Rendering::Mesh2D(spaceTransformer.pixelsToMeters(renderer.getWindowSize().x), spaceTransformer.pixelsToMeters(renderer.getWindowSize().y)));
    registry.add(m_deathScreen, Rendering::Material(DEATH_SCREEN_COLOR));
    registry.add(m_deathScreen, Rendering::Renderable{true, false});

    auto &sceneGraph = world.getSceneGraph();
    sceneGraph.relate(renderer.getActiveCamera(registry), m_deathScreen);

    m_diedText = registry.create();

    Rendering::Text diedText("You Died!", m_font);
    registry.add(m_diedText, diedText.mesh());

    auto &diedT = registry.add(m_diedText, Core::Transform(glm::vec2(0, 1.0f)));
    diedT.setZIndex(DEATH_SCREEN_LAYER + 1);
    diedT.scale(1.5f);

    registry.add(m_diedText, Rendering::Material(DEATH_SCREEN_TEXT_COLOR));
    registry.add(m_diedText, Rendering::Renderable{true, false});

    sceneGraph.relate(m_deathScreen, m_diedText);

    m_waveText = registry.create();

    Rendering::Text waveText("you made it to wave " + std::to_string(m_spawner.getWave()), m_font);
    registry.add(m_waveText, waveText.mesh());

    auto &waveT = registry.add(m_waveText, Core::Transform(glm::vec2(0, -1.0f)));
    waveT.setZIndex(DEATH_SCREEN_LAYER + 1);

    registry.add(m_waveText, Rendering::Material(DEATH_SCREEN_TEXT_COLOR));
    registry.add(m_waveText, Rendering::Renderable{true, false});

    sceneGraph.relate(m_deathScreen, m_waveText);
}

void DeathScreen::updateDeathScreen(const Core::Timestep &timestep)
{
    auto &world = *m_engine.getWorld();
    auto &registry = world.getRegistry();
    auto &renderer = *m_engine.getRenderer();
    auto &spaceTransformer = *m_engine.getSpaceTransformer();

    auto &t = registry.get<Core::Transform>(m_deathScreen);
    auto &mesh = registry.get<Rendering::Mesh2D>(m_deathScreen);

    auto &material = registry.get<Rendering::Material>(m_deathScreen);
    auto &diedMat = registry.get<Rendering::Material>(m_diedText);
    auto &waveMat = registry.get<Rendering::Material>(m_waveText);

    m_fadeInTimer += timestep.getSeconds();
    auto color = DEATH_SCREEN_COLOR;
    auto textColor = DEATH_SCREEN_TEXT_COLOR;

    if (m_fadeInTimer < m_fadeInDuration)
    {
        auto alpha = m_fadeInTimer / m_fadeInDuration;
        color.setA(alpha);
        textColor.setA(alpha);
    }
    else
    {
        color.setA(1.0f);
        textColor.setA(1.0f);
    }

    material.setColor(color);
    waveMat.setColor(textColor);
    diedMat.setColor(textColor);
}
