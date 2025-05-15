#include "RoboCatClientPCH.hpp"


MenuState::MenuState(StateStack& stack)
	:State(stack)
{
    m_background_sprite.setTexture(*TextureManager::sInstance->GetTexture("title"));
    auto play_button = std::make_shared<gui::Button>();
    play_button->setPosition(100, 300);
    play_button->SetText("Join Game");
    play_button->SetCallback([this]()
        {
            RequestStackPop();
            RequestStackPush(StateID::kCredentialEntryJoin);
        });

    auto host_play_button = std::make_shared<gui::Button>();
    host_play_button->setPosition(100, 350);
    host_play_button->SetText("Host Game");
    host_play_button->SetCallback([this]()
        {
            RequestStackPop();
            RequestStackPush(StateID::kCredentialEntryHost);
        });

    //auto settings_button = std::make_shared<gui::Button>();
    //settings_button->setPosition(100, 400);
    //settings_button->SetText("Settings");
    //settings_button->SetCallback([this]()
    //    {
    //        RequestStackPush(StateID::kSettings);
    //    });

    auto exit_button = std::make_shared<gui::Button>();
    exit_button->setPosition(100, 500);
    exit_button->SetText("Exit");
    exit_button->SetCallback([this]()
        {
            RequestStackPop();
        });

    m_container.Pack(play_button);
    m_container.Pack(host_play_button);
    //m_container.Pack(settings_button);
    m_container.Pack(exit_button);

    //Play the music
    //context.music->Play(MusicThemes::kMenuTheme);
}

void MenuState::Draw()
{
    WindowManager::sInstance->draw(m_background_sprite);
    WindowManager::sInstance->draw(m_container);
}

bool MenuState::Update(float dt)
{
    return true;
}

bool MenuState::HandleEvent(const sf::Event& event)
{
    m_container.HandleEvent(event);
    return false;
}
