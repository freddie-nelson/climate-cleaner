#pragma once

#include "Dialogue.h"

#include <remi/Engine.h>
#include <vector>

#define WIZARD_DIALOGUE_POSITION glm::vec2(0, 1.5f)
#define WIZARD_WIDTH 0.5f
#define WIZARD_HEIGHT 1.0f
#define WIZARD_COLOR Rendering::Color(0.6f, 0.0f, 0.7f, 1.0f)

class WizardNpc : public World::System
{

public:
    WizardNpc(remi::Engine &engine, glm::vec2 position = glm::vec2(0));

    void update(World::World &world, const Core::Timestep &timestep) override;

private:
    remi::Engine &m_engine;

    ECS::Entity m_wizardNpc;

    float m_dialogueDelay = 3.0f;
    float m_dialogueStartTimer = 0.0f;
    bool m_dialogueStarted = false;
    bool m_dialogueFinished = false;

    Dialogue *m_dialogue = nullptr;

    const std::vector<DialogueOption> m_dialogueOptions =
        {
            {"Wizard", "Hello, I am the grand climate change wizard.", 3.0f, 1.0f},
            {"Wizard", "You are here to discover how to save the world.", 3.0f, 1.0f},
            {"Wizard", "You must discover the places where the climate is dying.", 3.0f, 1.0f},
            {"Wizard", "And save them to unlock some climate change knowledge", 3.0f, 1.0f},
            {"Wizard", "Press 'F' to start your journey!", 60.0f, 1.0f},
        };
};