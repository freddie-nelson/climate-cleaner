#include <numbers>

#include "../include/EnemySpawner.h"
#include "../include/Random.h"

EnemySpawner::EnemySpawner(remi::Engine &engine, ECS::Entity target)
    : m_engine(engine), m_target(target)
{
    auto &world = *m_engine.getWorld();
    world.addSystem(this);

    srand(time(NULL));
}

void EnemySpawner::fixedUpdate(World::World &world, const Core::Timestep &timestep)
{
    m_waveTimer += timestep.getSeconds();
    if (m_waveTimer >= m_waveDuration)
    {
        m_waveTimer = 0.0;
        m_wave++;
        std::cout << "Wave: " << m_wave << std::endl;
    }

    m_spawnTimer += timestep.getSeconds();

    spawnBee(world, timestep);

    for (auto it = m_enemies.begin(); it != m_enemies.end();)
    {
        if ((*it)->isDead())
        {
            delete *it;
            it = m_enemies.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void EnemySpawner::spawnBee(World::World &world, const Core::Timestep &timestep)
{
    if (m_spawnTimer - m_lastBeeSpawn <= getBeeSpawnRate())
    {
        return;
    }

    float radius = randomFloat(m_minSpawnRadius, m_maxSpawnRadius);
    float angle = randomFloat(0, 2 * std::numbers::pi);

    float x = radius * cos(angle);
    float y = radius * sin(angle);

    auto &registry = world.getRegistry();
    auto &sceneGraph = world.getSceneGraph();
    auto transform = Core::Transform(sceneGraph.getModelMatrix(m_target));

    m_lastBeeSpawn = m_spawnTimer;
    m_enemies.emplace(new Bee(m_engine, m_target, glm::vec2(x, y) + transform.getTranslation(), m_wave));

    // std::cout << "Spawned bee at (" << x << ", " << y << ")" << std::endl;
}

double EnemySpawner::getBeeSpawnRate()
{
    auto spawnRate = 1.0 - (m_wave * 0.05);
    if (spawnRate < 0.5)
    {
        spawnRate = 0.5;
    }

    return spawnRate;
}