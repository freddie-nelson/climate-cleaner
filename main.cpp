#include "include/Player.h"
#include "include/Floor.h"
#include "include/WizardNpc.h"
#include "include/EnemySpawner.h"
#include "include/PowerUpSpawner.h"
#include "include/WaveUI.h"
#include "include/DeathScreen.h"

#include <remi/Engine.h>
#include <remi/Rendering/Texture/TextureAtlas.h>

int main()
{
    Rendering::TextureAtlas::MAX_ATLAS_SIZE = 2048;

    // create engine
    remi::EngineConfig config;
    config.physicsWorldConfig.gravity = glm::vec2(0);

    remi::Engine engine(config);

    auto &renderer = *engine.getRenderer();
    renderer.setClearColor(Rendering::Color(1.0f, 0.76f, 0.86f, 1.0f));

    // create player
    Player player(engine);

    // create enemy spawner
    EnemySpawner spawner(engine, player);

    // create floor
    Floor floor(engine, player.getEntity());

    // create power up spawner
    PowerUpSpawner powerUpSpawner(engine, player, floor, spawner);

    // create wave ui
    WaveUI waveUI(engine, spawner);

    // create death screen
    DeathScreen deathScreen(engine, player, spawner);

    // run engine
    engine.run();

    return 0;
}