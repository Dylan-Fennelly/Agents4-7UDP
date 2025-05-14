#pragma once
class GameState final : public State
{
public:
	explicit GameState(StateStack& stack);
	bool Update(float dt) override;
	bool HandleEvent(const sf::Event& event) override;
	void Draw() override;
private:
	bool m_networkInitialised = false;
};

