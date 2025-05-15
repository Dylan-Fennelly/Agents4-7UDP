/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

#pragma once
class GameState final : public State
{
public:
	explicit GameState(StateStack& stack,bool isHosting);
	bool Update(float dt) override;
	bool HandleEvent(const sf::Event& event) override;
	void Draw() override;
	void OnStackPopped() override;
private:
	bool m_networkInitialised = false;
	bool m_isHost = false;
};

