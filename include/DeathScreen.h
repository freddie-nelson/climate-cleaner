#pragma once

#include "EnemySpawner.h"
#include "Player.h"

#include <remi/Engine.h>
#include <remi/Rendering/Font/Font.h>
#include <remi/Rendering/Font/Text.h>

#define DEATH_SCREEN_COLOR Rendering::Color(0.6f, 0.0f, 0.0f, 0.0f)
#define DEATH_SCREEN_TEXT_COLOR Rendering::Color(1.0f, 1.0f, 1.0f, 1.0f)

class DeathScreen : public World::System
{
public:
    DeathScreen(remi::Engine &engine, Player &player, EnemySpawner &spawner);

    void update(World::World &world, const Core::Timestep &timestep) override;

private:
    remi::Engine &m_engine;
    Player &m_player;
    EnemySpawner &m_spawner;

    ECS::Entity m_deathScreen;
    bool m_dead = false;
    float m_fadeInTimer = 0.0f;
    float m_fadeInDuration = 3.0f;

    Rendering::Font m_font = Rendering::Font("assets/fonts/Micro5-Regular.ttf");

    ECS::Entity m_diedText;
    ECS::Entity m_waveText;

    void createDeathScreen();
    void updateDeathScreen(const Core::Timestep &timestep);
};
