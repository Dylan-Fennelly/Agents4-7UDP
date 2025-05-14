#include "RoboCatClientPCH.hpp"

TitleState::TitleState(StateStack& stack)
	: State(stack)
	, m_show_text(true)
	, m_text_effect_time(0)
{
	const auto texture_ptr = TextureManager::sInstance->GetTexture("title");
	m_background_sprite.setTexture(*texture_ptr);

	const auto position = WindowManager::sInstance->getView().getSize() / 2.f;

	std::shared_ptr<gui::Label> label;
	label = std::make_shared<gui::Label>("Press any key to continue");
	label->setPosition(position.x, position.y);
	//Utility::CreateLabel(label, position.x, position.y, "Press any key to continue", 50);

	m_container.Pack(label);
}

bool TitleState::Update(const float dt)
{
	m_text_effect_time += dt;

	if (m_text_effect_time >= .5f)
	{
		m_show_text = !m_show_text;
		m_text_effect_time = 0;
	}
	return true;
}
bool TitleState::HandleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyReleased)
	{
		RequestStackPop();
		RequestStackPush(StateID::kGame);
	}
	return true;
}

void TitleState::Draw()
{
	WindowManager::sInstance->draw(m_background_sprite);
	if (m_show_text)
	{
		WindowManager::sInstance->draw(m_container);
	}
}
