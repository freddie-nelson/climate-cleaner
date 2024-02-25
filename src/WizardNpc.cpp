#include "../include/WizardNpc.h"

#include <remi/Rendering/Renderable.h>
#include <remi/Rendering/Mesh/Mesh.h>
#include <remi/Rendering/Material/Material.h>
#include <remi/Core/Transform.h>
#include <remi/Physics/RigidBody2D.h>
#include <remi/Physics/Collider2D.h>

WizardNpc::WizardNpc(remi::Engine &engine, glm::vec2 position) : m_engine(engine)
{
    auto &world = *engine.getWorld();
    auto &registry = world.getRegistry();
    auto &sceneGraph = world.getSceneGraph();

    m_wizardNpc = registry.create();
    registry.add(m_wizardNpc, Core::Transform(position));
    registry.add(m_wizardNpc, Rendering::Mesh2D(WIZARD_WIDTH, WIZARD_HEIGHT));
    registry.add(m_wizardNpc, Rendering::Material(WIZARD_COLOR));
    registry.add(m_wizardNpc, Rendering::Renderable(true, false));

    auto &body = registry.add(m_wizardNpc, Physics::RigidBody2D());
    body.setFixedRotation(true);

    auto wizardColliderShape = Physics::PolygonColliderShape2D(Rendering::Mesh2D(WIZARD_WIDTH, WIZARD_HEIGHT));
    auto &collider = registry.add(m_wizardNpc, Physics::Collider2D(&wizardColliderShape));
    collider.setDensity(10.0f);
    collider.setFriction(1.0f);

    world.addSystem(this);
}

void WizardNpc::update(World::World &world, const Core::Timestep &timestep)
{
    m_dialogueStartTimer += timestep.getSeconds();

    if (!m_dialogueStarted && m_dialogueStartTimer >= m_dialogueDelay)
    {
        m_dialogueStartTimer = 0.0f;
        m_dialogueStarted = true;

        m_dialogue = new Dialogue(m_engine, m_dialogueOptions, WIZARD_DIALOGUE_POSITION, &m_wizardNpc);
        m_dialogue->setBeginDialogueCallback([this](DialogueOption dialogueOption, bool lastDialogue)
                                             {
                                            if (lastDialogue) m_dialogueFinished = true; });
    }

    if (m_dialogueFinished)
    {
        auto &keyboard = *m_engine.getKeyboard();

        if (keyboard.isPressed(Input::Key::F))
        {
            m_dialogue->stop();
            m_dialogue = nullptr;

            std::cout << "F pressed" << std::endl;
            world.removeSystem(this);
        }
    }
}
