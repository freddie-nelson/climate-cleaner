#pragma once

#include "EnemySpawner.h"

#include <remi/Engine.h>
#include <remi/Rendering/Font/Font.h>

#define WAVE_TEXT "Wave: "
#define WAVE_TEXT_COLOR Rendering::Color(1.0f, 1.0f, 1.0f, 1.0f)
#define WAVE_TEXT_PADDING glm::vec2(10.0f, 10.0f)
#define WAVE_TEXT_SCALE 0.75f

#define WAVE_ANNOUNCEMENT_SCALE 2.25f
#define WAVE_ANNOUNCEMENT_COLOR Rendering::Color(1.0f, 1.0f, 1.0f, 1.0f)

class WaveUI : public World::System
{
public:
    WaveUI(remi::Engine &engine, EnemySpawner &spawner);

    void update(World::World &world, const Core::Timestep &timestep) override;

private:
    remi::Engine &m_engine;
    EnemySpawner &m_spawner;

    int m_lastWave = 0;
    Rendering::Font m_font = Rendering::Font("assets/fonts/Micro5-Regular.ttf");

    ECS::Entity m_activeCamera;
    ECS::Entity m_waveText;

    ECS::Entity m_waveAnnouncement;
    bool m_announcing = false;
    float m_announcementTimer = 0.0f;
    float m_announcementDuration = 6.0f;

    void createWaveText();
    void updateWaveText();

    void announceWave(int wave);
    void updateAnnouncement(const Core::Timestep &timestep);
    void destroyAnnouncement();
};
