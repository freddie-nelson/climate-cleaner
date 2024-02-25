#include "../include/Dialogue.h"

#include <remi/Rendering/Renderable.h>
#include <remi/Rendering/Material/Material.h>
#include <remi/Core/Transform.h>

Dialogue::Dialogue(remi::Engine &engine, std::vector<DialogueOption> dialogue, glm::vec2 position, ECS::Entity *parent)
    : m_engine(engine), m_dialogue(dialogue), m_position(position), m_parent(parent)
{
    m_font = new Rendering::Font(DIALOGUE_FONT);

    auto &world = *m_engine.getWorld();
    world.addSystem(this);
}

void Dialogue::update(World::World &world, const Core::Timestep &timestep)
{

    if (!m_createdDialogueEntity && m_timer == 0.0f)
    {
        createDialogueEntity();
        m_createdDialogueEntity = true;

        if (m_beginDialogueCallback != nullptr)
        {
            m_beginDialogueCallback(m_currentDialogue, m_dialogue.empty());
        }
    }

    m_timer += timestep.getSeconds();

    if (m_timer >= m_currentDialogue.duration)
    {
        destroyDialogueEntity();

        if (m_endDialogueCallback != nullptr)
        {
            m_endDialogueCallback(m_currentDialogue, m_dialogue.empty());
        }
    }

    if (m_timer >= m_currentDialogue.duration + m_currentDialogue.pause)
    {
        if (m_dialogue.empty())
        {
            world.removeSystem(this);
            delete this;
            return;
        }

        m_timer = 0.0f;
    }
}

void Dialogue::stop()
{
    destroyDialogueEntity();

    auto &world = *m_engine.getWorld();
    world.removeSystem(this);

    m_endDialogueCallback = nullptr;
    m_beginDialogueCallback = nullptr;

    delete this;
}

void Dialogue::setBeginDialogueCallback(std::function<void(DialogueOption, bool)> callback)
{
    m_beginDialogueCallback = callback;
}

void Dialogue::setEndDialogueCallback(std::function<void(DialogueOption, bool)> callback)
{
    m_endDialogueCallback = callback;
}

void Dialogue::createDialogueEntity()
{
    if (m_dialogue.empty())
    {
        return;
    }

    auto &world = *m_engine.getWorld();
    auto &registry = world.getRegistry();
    auto &sceneGraph = world.getSceneGraph();

    if (m_createdDialogueEntity)
    {
        registry.destroy(m_dialogueEntity);
    }

    m_currentDialogue = m_dialogue.front();
    m_dialogue.erase(m_dialogue.begin());

    m_dialogueEntity = registry.create();
    registry.add(m_dialogueEntity, Rendering::Renderable(true, false));
    registry.add(m_dialogueEntity, Rendering::Material(DIALOGUE_COLOR));

    auto text = Rendering::Text(m_currentDialogue.text, *m_font);
    registry.add(m_dialogueEntity, text.mesh());

    auto &transform = registry.add(m_dialogueEntity, Core::Transform(m_position));
    transform.scale(DIALOGUE_SCALE);

    if (m_parent != nullptr)
    {
        sceneGraph.relate(*m_parent, m_dialogueEntity);
    }
}

void Dialogue::destroyDialogueEntity()
{
    auto &world = *m_engine.getWorld();
    auto &registry = world.getRegistry();

    if (m_createdDialogueEntity)
    {
        registry.destroy(m_dialogueEntity);
        m_createdDialogueEntity = false;
    }
}