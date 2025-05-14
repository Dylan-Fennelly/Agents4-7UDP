#include "RoboCatClientPCH.hpp"
gui::Label::Label(const std::string& text)
    :m_text(text, *FontManager::sInstance->GetFont("carlito"), 16)
{
}
bool gui::Label::IsSelectable() const
{
    return false;
}
void gui::Label::SetText(const std::string & text)
{
    m_text.setString(text);
}

void gui::Label::HandleEvent(const sf::Event& event)
{
}
void gui::Label::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(m_text, states);
}
