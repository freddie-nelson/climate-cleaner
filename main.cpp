#include "include/Player.h"
#include "include/Floor.h"

#include <remi/Engine.h>
#include <remi/Rendering/Texture/TextureAtlas.h>

int main()
{
    Rendering::TextureAtlas::MAX_ATLAS_SIZE = 2048;

    // create engine
    remi::EngineConfig config;
    remi::Engine engine(config);

    // create player
    Player player(engine);

    // create floor
    Floor floor(engine, glm::vec2(0, -5));

    // run engine
    engine.run();

    return 0;
}