#pragma once
class CredentialEntryState : public State
{
public:
	CredentialEntryState(StateStack& stack, bool is_host);
	virtual void Draw() override;
	virtual bool Update(float dt) override;
	virtual bool HandleEvent(const sf::Event& event) override;

private:
	sf::Sprite m_background_sprite;
	gui::Container m_gui_container;

	std::vector<TextBox> m_text_boxes;
	int m_text_box_Selected = -1;
	bool m_is_host;

};