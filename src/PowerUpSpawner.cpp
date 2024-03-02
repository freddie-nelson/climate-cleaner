#include "../include/PowerUpSpawner.h"
#include "../include/Random.h"
#include "../include/HealthPowerUp.h"
#include "../include/RocketLauncherPowerUp.h"
#include "../include/FreezeGunPowerUp.h"

PowerUpSpawner::PowerUpSpawner(remi::Engine &engine, Player &player, const Floor &floor)
    : m_engine(engine), m_player(player), m_floor(floor)
{
    auto &world = *m_engine.getWorld();
    world.addSystem(this);
}

void PowerUpSpawner::update(World::World &world, const Core::Timestep &timestep)
{
    m_spawnTimer -= timestep.getSeconds();

    // spawn power ups
    if (m_spawnTimer <= 0.0f)
    {
        spawnPowerUp();
        m_spawnTimer = m_spawnTime;
    }

    // update power ups
    for (auto &powerUp : m_powerUps)
    {
        powerUp->update(world, timestep);
    }

    // update active power ups
    for (auto &pair : m_activePowerUps)
    {
        pair.second->update(world, timestep);
    }

    // check for picked up power ups
    auto &registry = world.getRegistry();
    auto &transform = registry.get<Core::Transform>(m_player.getEntity());

    for (auto it = m_powerUps.begin(); it != m_powerUps.end();)
    {
        auto powerUp = *it;
        auto type = powerUp->getType();

        if (glm::distance(powerUp->getPosition(), transform.getTranslation()) < POWERUP_PICKUP_RADIUS)
        {
            // remove incompatible active power ups
            for (auto &incompatible : powerUp->getIncompatiblePowerUps())
            {
                if (m_activePowerUps.contains(incompatible))
                {
                    if (incompatible != type)
                        m_activePowerUps[incompatible]->remove(m_player);

                    delete m_activePowerUps[incompatible];
                    m_activePowerUps.erase(incompatible);
                }
            }

            powerUp->destroy();
            powerUp->apply(m_player);

            it = m_powerUps.erase(it);

            m_activePowerUps[type] = powerUp;
        }
        else
        {
            ++it;
        }
    }

    // remove expired active power ups
    for (auto it = m_activePowerUps.begin(); it != m_activePowerUps.end();)
    {
        auto powerUp = it->second;

        if (powerUp->isExpired())
        {
            powerUp->remove(m_player);
            delete powerUp;
            it = m_activePowerUps.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // remove expired power ups
    for (auto it = m_powerUps.begin(); it != m_powerUps.end();)
    {
        auto powerUp = *it;

        if (powerUp->isExpired())
        {
            delete *it;
            it = m_powerUps.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void PowerUpSpawner::spawnPowerUp()
{
    if (randomFloat(0.0f, 1.0f) > m_spawnChance)
    {
        return;
    }

    auto type = getRandomPowerUpType();
    auto powerUp = createPowerUp(type);

    if (powerUp == nullptr)
    {
        return;
    }

    m_powerUps.push_back(powerUp);
}

glm::vec2 PowerUpSpawner::getRandomPosition()
{
    auto &registry = m_engine.getWorld()->getRegistry();
    auto &transform = registry.get<Core::Transform>(m_player.getEntity());

    auto max = m_floor.getFloorMax();
    auto min = m_floor.getFloorMin();

    auto distance = randomFloat(m_minSpawnRadius, m_maxSpawnRadius);
    auto angle = randomFloat(0.0f, 2 * std::numbers::pi);
    glm::vec2 direction = {std::cos(angle), std::sin(angle)};

    glm::vec2 position = transform.getTranslation() + direction * distance;

    return glm::clamp(position, min, max);
}

PowerUpType PowerUpSpawner::getRandomPowerUpType()
{
    auto i = randomInt(1, 100);

    if (i <= 15)
    {
        return PowerUpType::HEALTH_POWERUP;
    }
    else if (i <= 30)
    {
        return PowerUpType::ROCKET_LAUNCHER_POWERUP;
    }
    else
    {
        return PowerUpType::FREEZE_GUN_POWERUP;
    }
}

PowerUp *PowerUpSpawner::createPowerUp(PowerUpType type)
{
    auto position = getRandomPosition();
    auto timeToLive = randomFloat(m_spawnTime * 0.5f, m_spawnTime * 1.5f);

    switch (type)
    {
    case PowerUpType::HEALTH_POWERUP:
        return new HealthPowerUp(m_engine, position, timeToLive);
    case PowerUpType::FREEZE_GUN_POWERUP:
        return new FreezeGunPowerUp(m_engine, position, timeToLive);
    case PowerUpType::ROCKET_LAUNCHER_POWERUP:
        return new RocketLauncherPowerUp(m_engine, position, timeToLive);
    default:
        return nullptr;
    }
}