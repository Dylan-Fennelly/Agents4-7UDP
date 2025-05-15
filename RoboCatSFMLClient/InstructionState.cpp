#include "RoboCatClientPCH.hpp"

InstructionState::InstructionState(StateStack& stack,bool isHosting)
    : State(stack)
	, m_is_host(isHosting)
{
    // Load the instruction image
    m_instruction_sprite.setTexture(*TextureManager::sInstance->GetTexture("instructions")); // Make sure "instructions" is registered

    // Button setup
    const float centerX = WindowManager::sInstance->getView().getSize().x / 2.f;
    const float buttonY = WindowManager::sInstance->getView().getSize().y - 100.f;

    auto back_button = std::make_shared<gui::Button>();
    back_button->SetText("Back to Menu");
    back_button->setPosition(centerX - 220.f, buttonY);
    back_button->SetCallback([this]() {
        RequestStackClear();
        RequestStackPush(StateID::kMenu);
        });

    auto continue_button = std::make_shared<gui::Button>();
    continue_button->SetText("Continue");
    continue_button->setPosition(centerX + 20.f, buttonY);

    continue_button->SetCallback([this]() {
        StateID stateID = m_is_host ? StateID::kCredentialEntryHost : StateID::kCredentialEntryJoin;
        RequestStackClear();
        RequestStackPush(stateID);
        });

    m_container.Pack(back_button);
    m_container.Pack(continue_button);
}

void InstructionState::Draw()
{
    WindowManager::sInstance->draw(m_instruction_sprite);
    WindowManager::sInstance->draw(m_container);
}

bool InstructionState::Update(float)
{
    return true;
}

bool InstructionState::HandleEvent(const sf::Event& event)
{
    m_container.HandleEvent(event);
    return false;
}
