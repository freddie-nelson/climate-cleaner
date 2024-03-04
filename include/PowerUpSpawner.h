#pragma once

#include "Floor.h"
#include "Player.h"
#include "PowerUp.h"
#include "EnemySpawner.h"

#include <remi/Engine.h>
#include <unordered_map>
#include <list>

class PowerUpSpawner : public World::System
{
public:
    PowerUpSpawner(remi::Engine &engine, Player &player, const Floor &floor, EnemySpawner &enemySpawner);

    void update(World::World &world, const Core::Timestep &timestep) override;

private:
    remi::Engine &m_engine;
    Player &m_player;
    const Floor &m_floor;
    EnemySpawner &m_enemySpawner;

    float m_spawnChance = 0.5f;

    float m_spawnTime = 0.005f;
    // float m_spawnTime = 5.0f;
    float m_powerUpTime = 15.0f;
    float m_spawnTimer = m_spawnTime * 2.0f;
    float m_minSpawnRadius = 3.0f;
    float m_maxSpawnRadius = 9.0f;

    std::list<PowerUp *> m_powerUps;
    std::unordered_map<PowerUpType, PowerUp *> m_activePowerUps;

    void spawnPowerUp();

    glm::vec2 getRandomPosition();

    PowerUpType getRandomPowerUpType();

    PowerUp *createPowerUp(PowerUpType type);
};