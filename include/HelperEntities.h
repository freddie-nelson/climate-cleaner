#pragma once

#include "Layers.h"

#include <remi/ECS/Entity.h>
#include <remi/World/World.h>
#include <remi/Core/Transform.h>
#include <remi/Rendering/Renderable.h>
#include <remi/Rendering/Mesh/Mesh.h>
#include <remi/Rendering/Material/Material.h>

inline ECS::Entity createShadow(World::World &world, ECS::Entity owner, glm::vec2 position, float radius, glm::vec2 scale, Rendering::Color color = Rendering::Color(0, 0, 0, 0.3f))
{
    auto &registry = world.getRegistry();
    auto &sceneGraph = world.getSceneGraph();

    auto shadow = registry.create();
    auto &shadowT = registry.add(shadow, Core::Transform(position));
    shadowT.setScale(scale);
    shadowT.setZIndex(SHADOW_LAYER);

    registry.add(shadow, Rendering::Mesh2D(radius, 32u));
    registry.add(shadow, Rendering::Material(color));
    registry.add(shadow, Rendering::Renderable(true, false));

    sceneGraph.relate(owner, shadow);

    return shadow;
}