#include "RoboCatClientPCH.hpp"

gui::Button::Button()
    : m_sprite(*TextureManager::sInstance->GetTexture("button"))
    , m_text("", *FontManager::sInstance->GetFont("carlito"), 16)
    , m_is_toggle(false)
{
    ChangeTexture(ButtonType::kNormal);

    // Centre the text correctly
    sf::FloatRect spriteBounds = m_sprite.getLocalBounds();
    sf::FloatRect textBounds = m_text.getLocalBounds();

    // Origin: half of width/height only (don't add left/top)
    m_text.setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);

    // Position: centre of the button
    m_text.setPosition(spriteBounds.width / 2.f, spriteBounds.height / 2.f + 2.f); // +2.f for visual centring
}

void gui::Button::SetCallback(Callback callback)
{
    m_callback = std::move(callback);
}

void gui::Button::SetText(const std::string& text)
{
    m_text.setString(text);

    // Recalculate bounds for new text
    sf::FloatRect textBounds = m_text.getLocalBounds();
    m_text.setOrigin(textBounds.width / 2.f, textBounds.height / 2.f);

    sf::FloatRect spriteBounds = m_sprite.getLocalBounds();
    m_text.setPosition(spriteBounds.width / 2.f, spriteBounds.height / 2.f + 2.f);
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

    // AudioManager::sInstance->Play("Button"); // optional
}

void gui::Button::Deactivate()
{
    Component::Deactivate();

    if (m_is_toggle)
    {
        if (IsSelected())
            ChangeTexture(ButtonType::kSelected);
        else
            ChangeTexture(ButtonType::kNormal);
    }
}

void gui::Button::HandleEvent(const sf::Event& event)
{
    // No input handling defined here
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
