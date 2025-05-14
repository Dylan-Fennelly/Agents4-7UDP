#include "RoboCatClientPCH.hpp"

Title2::Title2(StateStack& stack)
	: State(stack)
	, m_show_text(true)
	, m_text_effect_time(0)
{
	const auto texture_ptr = TextureManager::sInstance->GetTexture("title");
	m_background_sprite.setTexture(*texture_ptr);

	const auto position = WindowManager::sInstance->getView().getSize() / 2.f;

	std::shared_ptr<gui::Label> label;
	label = std::make_shared<gui::Label>("you have arrived",26,sf::Color::Red);
	label->setPosition(position.x, position.y);
	//Utility::CreateLabel(label, position.x, position.y, "Press any key to continue", 50);

	m_container.Pack(label);
}

bool Title2::Update(const float dt)
{
	m_text_effect_time += dt;

	if (m_text_effect_time >= .5f)
	{
		m_show_text = !m_show_text;
		m_text_effect_time = 0;
	}
	return true;
}
bool Title2::HandleEvent(const sf::Event& event)
{
	if (event.type == sf::Event::KeyReleased)
	{
		RequestStackPop();
		RequestStackPush(StateID::kGame);
	}
	return true;
}

void Title2::Draw()
{
	WindowManager::sInstance->draw(m_background_sprite);
	if (m_show_text)
	{
		WindowManager::sInstance->draw(m_container);
	}
}
