/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

#include "RoboCatClientPCH.hpp"

TitleState::TitleState(StateStack& stack)
	: State(stack)
	, m_show_text(true)
	, m_text_effect_time(0)
{
	// Set the background texture
	m_background_sprite.setTexture(*TextureManager::sInstance->GetTexture("title"));

	// Get view size to position elements
	const auto view_size = WindowManager::sInstance->getView().getSize();

	// Create the label
	std::shared_ptr<gui::Label> label = std::make_shared<gui::Label>("Press any key to continue", 32, sf::Color::Red);

	// Centre the origin of the text for proper alignment
	sf::FloatRect bounds = label->GetText().getLocalBounds();
	label->setOrigin(bounds.width / 2.f, bounds.height / 2.f);

	// Position the label in the lower-centre of the screen (e.g. 85% down)
	label->setPosition(view_size.x / 2.f, view_size.y * 0.85f);

	// Add the label to the container
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
		RequestStackPush(StateID::kMenu);
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
