#include "../include/WaveUI.h"
#include "../include/Layers.h"

#include <remi/Rendering/Font/Text.h>
#include <remi/Rendering/Material/Material.h>
#include <remi/Core/Transform.h>
#include <remi/Rendering/Camera/ActiveCamera.h>
#include <remi/Rendering/Renderable.h>
#include <remi/Rendering/Camera/Camera.h>
#include <string>

WaveUI::WaveUI(remi::Engine &engine, EnemySpawner &enemySpawner) : m_engine(engine), m_spawner(enemySpawner)
{
    createWaveText();

    auto &world = *m_engine.getWorld();
    world.addSystem(this);
}

void WaveUI::update(World::World &world, const Core::Timestep &timestep)
{
    updateWaveText();
    updateAnnouncement(timestep);

    m_lastWave = m_spawner.getWave();
}

void WaveUI::createWaveText()
{
    auto &world = *m_engine.getWorld();
    auto &registry = world.getRegistry();
    auto &renderer = *m_engine.getRenderer();

    m_activeCamera = renderer.getActiveCamera(registry);

    m_waveText = registry.create();
    auto &t = registry.add(m_waveText, Core::Transform());
    t.setZIndex(WAVEUI_LAYER);
    t.scale(WAVE_TEXT_SCALE);

    registry.add(m_waveText, Rendering::Material(WAVE_TEXT_COLOR));
    registry.add(m_waveText, Rendering::Renderable{true, false});

    auto &sceneGraph = world.getSceneGraph();
    sceneGraph.relate(m_activeCamera, m_waveText);
}

void WaveUI::updateWaveText()
{
    auto &world = *m_engine.getWorld();
    auto &registry = world.getRegistry();
    auto &spaceTransformer = *m_engine.getSpaceTransformer();
    auto &renderer = *m_engine.getRenderer();

    auto &camera = registry.get<Rendering::Camera>(m_activeCamera);
    auto viewport = renderer.getWindowSize();

    if (m_lastWave != m_spawner.getWave())
    {
        auto text = Rendering::Text(WAVE_TEXT + std::to_string(m_spawner.getWave()), m_font);
        auto &mesh = text.mesh();
        registry.add(m_waveText, mesh);

        auto &t = registry.get<Core::Transform>(m_waveText);

        auto aabb = mesh.getAABB().transform(t);

        t.setTranslation(spaceTransformer.transform(glm::vec2(WAVE_TEXT_PADDING.x, viewport.y - WAVE_TEXT_PADDING.y), m_waveText, Core::SpaceTransformer::SCREEN, Core::SpaceTransformer::LOCAL) + glm::vec2(aabb.getWidth() / 2.0f, -aabb.getHeight() / 2.0f));
    }
}

void WaveUI::updateAnnouncement(const Core::Timestep &timestep)
{
    if (!m_announcing)
    {
        if (m_lastWave != m_spawner.getWave())
        {
            announceWave(m_spawner.getWave());
        }

        return;
    }

    m_announcementTimer += timestep.getSeconds();

    if (m_announcementTimer >= m_announcementDuration)
    {
        m_announcing = false;
        m_announcementTimer = 0;
        destroyAnnouncement();
        return;
    }

    auto &world = *m_engine.getWorld();
    auto &registry = world.getRegistry();

    auto &material = registry.get<Rendering::Material>(m_waveAnnouncement);
    auto color = material.getColor();

    // fade in
    if (m_announcementTimer <= m_announcementDuration / 3.0f)
    {
        color.setA(1.0f * (m_announcementTimer / (m_announcementDuration / 3.0f)));
    }
    // stay at full alpha
    else if (m_announcementTimer <= m_announcementDuration * 2.0f / 3.0f)
    {
        color.setA(1.0f);
    }
    // fade out
    else if (m_announcementTimer <= m_announcementDuration)
    {
        color.setA(1.0f * (1 - (m_announcementTimer - (m_announcementDuration * 2.0f / 3.0f)) / (m_announcementDuration / 3.0f)));
    }

    material.setColor(color);
}

void WaveUI::announceWave(int wave)
{
    m_announcing = true;
    m_announcementTimer = 0.0f;

    auto &world = *m_engine.getWorld();
    auto &registry = world.getRegistry();
    auto &sceneGraph = world.getSceneGraph();

    m_waveAnnouncement = registry.create();
    auto &t = registry.add(m_waveAnnouncement, Core::Transform());
    t.setZIndex(WAVEUI_LAYER);
    t.scale(WAVE_ANNOUNCEMENT_SCALE);

    auto color = WAVE_ANNOUNCEMENT_COLOR;
    color.setA(0);

    registry.add(m_waveAnnouncement, Rendering::Material(color));
    registry.add(m_waveAnnouncement, Rendering::Renderable{true, false});

    auto text = Rendering::Text("Wave " + std::to_string(wave), m_font);
    auto &mesh = text.mesh();
    registry.add(m_waveAnnouncement, mesh);

    sceneGraph.relate(m_activeCamera, m_waveAnnouncement);
}

void WaveUI::destroyAnnouncement()
{
    auto &world = *m_engine.getWorld();
    auto &registry = world.getRegistry();

    registry.destroy(m_waveAnnouncement);
}