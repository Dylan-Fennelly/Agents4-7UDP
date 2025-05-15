/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

#pragma once
class Title2 final : public State
{
public:
	explicit Title2(StateStack& stack);
	bool Update(float dt) override;
	bool HandleEvent(const sf::Event& event) override;
	void Draw() override;

private:
	sf::Sprite m_background_sprite;

	bool m_show_text;
	float m_text_effect_time;
	SpriteComponentPtr mSpriteComponent;
	gui::Container m_container;
};
