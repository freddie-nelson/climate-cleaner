#pragma once

#include <remi/Engine.h>
#include <remi/Rendering/Font/Font.h>
#include <remi/Rendering/Font/Text.h>

#define DIALOGUE_FONT "assets/fonts/Micro5-Regular.ttf"
#define DIALOGUE_COLOR Rendering::Color(0.0f, 0.0f, 0.0f, 1.0f)
#define DIALOGUE_SCALE 0.5f

struct DialogueOption
{
    std::string name;
    std::string text;
    float duration;
    float pause;
};

class Dialogue : public World::System
{
public:
    Dialogue(remi::Engine &engine, std::vector<DialogueOption> dialogue, glm::vec2 position = glm::vec2(0), ECS::Entity *parent = nullptr);

    void update(World::World &world, const Core::Timestep &timestep) override;

    void setBeginDialogueCallback(std::function<void(DialogueOption, bool)> callback);
    void setEndDialogueCallback(std::function<void(DialogueOption, bool)> callback);

    void stop();

private:
    remi::Engine &m_engine;
    ECS::Entity *m_parent = nullptr;

    std::function<void(DialogueOption, bool)> m_beginDialogueCallback = nullptr;
    std::function<void(DialogueOption, bool)> m_endDialogueCallback = nullptr;

    std::vector<DialogueOption> m_dialogue;
    DialogueOption m_currentDialogue;
    float m_timer = 0.0f;

    Rendering::Font *m_font;

    bool m_createdDialogueEntity = false;
    ECS::Entity m_dialogueEntity;
    glm::vec2 m_position;

    void createDialogueEntity();
    void destroyDialogueEntity();
};