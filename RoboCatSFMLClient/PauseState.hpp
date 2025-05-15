/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

#pragma once
class PauseState : public State
{
public:
	PauseState(StateStack& stack,bool lets_updates_through = true);
	~PauseState();
	virtual void Draw() override;
	virtual bool Update(float dt) override;
	virtual bool HandleEvent(const sf::Event& event) override;

private:
	sf::Sprite m_background_sprite;
	sf::Text m_paused_text;
	gui::Container m_gui_container;
	bool m_lets_updates_through;
};
