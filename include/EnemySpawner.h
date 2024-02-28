#pragma once

#include "Bee.h"

#include <remi/Engine.h>
#include <unordered_set>

class EnemySpawner : public World::System
{
public:
    EnemySpawner(remi::Engine &engine, ECS::Entity target);

    void fixedUpdate(World::World &world, const Core::Timestep &timestep) override;

private:
    int m_wave = 0;
    double m_spawnTimer = 0.0;
    double m_lastBeeSpawn = 0.0;

    float m_minSpawnRadius = 5.0f;
    float m_maxSpawnRadius = 10.0f;

    remi::Engine &m_engine;
    ECS::Entity m_target;

    std::unordered_set<Enemy *> m_enemies;

    void spawnBee(World::World &world, const Core::Timestep &timestep);

    double getBeeSpawnRate();
};
