#include "include/Player.h"
#include "include/Floor.h"
#include "include/WizardNpc.h"

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
    renderer.setClearColor(Rendering::Color(0, 0.5f, 0.0f, 1.0f));

    // create player
    Player player(engine);

    // create wizard npc
    WizardNpc wizardNpc(engine, glm::vec2(0, 0));

    // run engine
    engine.run();

    return 0;
}