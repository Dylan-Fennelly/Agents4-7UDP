/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

#include "RoboCatClientPCH.hpp"

PauseState::PauseState(StateStack& stack, bool lets_updates_through)
	:State(stack)
	, m_lets_updates_through(lets_updates_through)

{

    FontPtr fontPtr = FontManager::sInstance->GetFont("carlito");
    sf::Vector2f view_size = WindowManager::sInstance->getView().getSize();

    m_paused_text.setFont(*fontPtr);
    m_paused_text.setString("Game Paused");
    m_paused_text.setCharacterSize(70);
    sf::FloatRect bounds = m_paused_text.getLocalBounds();
    m_paused_text.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
    m_paused_text.setPosition(0.5f * view_size.x, 0.4f * view_size.y);

    auto returnButton = std::make_shared<gui::Button>();
    returnButton->setPosition(0.5f * view_size.x - 100, 0.4f * view_size.y + 75);
    returnButton->SetText("Return");
    returnButton->SetCallback([this]()
        {
			InputManager::sInstance->UnPause();
            RequestStackPop();
        });

    auto backToMenuButton = std::make_shared<gui::Button>();
    backToMenuButton->setPosition(0.5f * view_size.x - 100, 0.4f * view_size.y + 125);
    backToMenuButton->SetText("Back to menu");
    backToMenuButton->SetCallback([this]()
        {
            RequestStackClear();
			InputManager::sInstance->UnPause(); //Need to unpause the input manager, it'll fuck up joining a new game otherwise
            RequestStackPush(StateID::kMenu);
        });

    m_gui_container.Pack(returnButton);
    m_gui_container.Pack(backToMenuButton);

    //Pause the music
   // GetContext().music->SetPaused(true);
}

void PauseState::Draw()
{


    sf::RectangleShape backgroundShape;
    backgroundShape.setFillColor(sf::Color(0, 0, 0, 150));
    backgroundShape.setSize(WindowManager::sInstance->getView().getSize());

    WindowManager::sInstance->draw(backgroundShape);
    WindowManager::sInstance->draw(m_paused_text);
    WindowManager::sInstance->draw(m_gui_container);
}

bool PauseState::Update(float dt)
{
    return m_lets_updates_through;
}

bool PauseState::HandleEvent(const sf::Event& event)
{
    m_gui_container.HandleEvent(event);

    return false;
}

PauseState::~PauseState()
{
    //GetContext().music->SetPaused(false);
}