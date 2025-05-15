/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

#pragma once

class MenuState final : public State
{
public:
	explicit MenuState(StateStack& stack);
	void Draw() override;
	bool Update(float dt) override;
	bool HandleEvent(const sf::Event& event) override;

private:
	sf::Sprite m_background_sprite;
	gui::Container m_container;
};


