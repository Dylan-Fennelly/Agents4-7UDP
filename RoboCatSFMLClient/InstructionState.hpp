#pragma once
#include "State.hpp"
#include "Container.hpp"

class InstructionState : public State
{
public:
    explicit InstructionState(StateStack& stack,bool isHosting);

    void Draw() override;
    bool Update(float dt) override;
    bool HandleEvent(const sf::Event& event) override;

private:
    sf::Sprite m_instruction_sprite;
    gui::Container m_container;
	bool m_is_host;
};
