#include "RoboCatClientPCH.hpp"

gui::Button::Button()
    : m_sprite(*TextureManager::sInstance->GetTexture("button"))
    , m_text("", *FontManager::sInstance->GetFont("carlito"), 16)
    , m_is_toggle(false)
{
    ChangeTexture(ButtonType::kNormal);
    sf::FloatRect bounds = m_sprite.getLocalBounds();
    m_text.setPosition(bounds.width / 2, bounds.height / 2);
}
void gui::Button::SetCallback(Callback callback)
{
    m_callback = std::move(callback);
}
void gui::Button::SetText(const std::string& text)
{
    m_text.setString(text);
}
void gui::Button::SetToggle(bool flag)
{
    m_is_toggle = flag;
}
bool gui::Button::IsSelectable() const
{
    return true;
}
void gui::Button::Select()
{
    Component::Select();
    ChangeTexture(ButtonType::kSelected);
}
void gui::Button::Deselect()
{
    Component::Deselect();
    ChangeTexture(ButtonType::kNormal);
}
void gui::Button::Activate()
{
	Component::Activate();
	//If toggle then show button is pressed or toggled
	if (m_is_toggle)
	{
		ChangeTexture(ButtonType::kPressed);
	}
	if (m_callback)
	{
		m_callback();
	}
	if (!m_is_toggle)
	{
		Deactivate();
	}

	//Todo: Implement AudioManager
	// AudioManager::sInstance->Play("Button");
}
void gui::Button::Deactivate()
{
    Component::Deactivate();
    if (m_is_toggle)
    {
        if (IsSelected())
        {
            ChangeTexture(ButtonType::kSelected);
        }
        else
        {
            ChangeTexture(ButtonType::kNormal);
        }
    }
}

void gui::Button::HandleEvent(const sf::Event& event)
{
}
void gui::Button::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(m_sprite, states);
    target.draw(m_text, states);
}
void gui::Button::ChangeTexture(ButtonType buttonType)
{
    sf::IntRect textureRect(0, 50 * static_cast<int>(buttonType), 200, 50);
    m_sprite.setTextureRect(textureRect);
}