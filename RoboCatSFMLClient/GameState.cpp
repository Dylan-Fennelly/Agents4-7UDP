#include "RoboCatClientPCH.hpp"

GameState::GameState(StateStack& stack):State(stack)
{
	InputManager::StaticInit();
}

bool GameState::Update(float dt)
{
	if (!m_networkInitialised)
	{
		std::string destination = StringUtils::GetCommandLineArg(1);
		std::string name = StringUtils::GetCommandLineArg(2);

		SocketAddressPtr serverAddress = SocketAddressFactory::CreateIPv4FromString(destination);
		NetworkManagerClient::StaticInit(*serverAddress, name);

		m_networkInitialised = true;
	}

	InputManager::sInstance->Update();
	//Engine::s_instance->DoFrame();
	World::sInstance->Update();
	NetworkManagerClient::sInstance->ProcessIncomingPackets();
	NetworkManagerClient::sInstance->SendOutgoingPackets();
	return true;
}

bool GameState::HandleEvent(const sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::KeyPressed:
		InputManager::sInstance->HandleInput(EIA_Pressed, event.key.code);
		break;
	case sf::Event::KeyReleased:
		InputManager::sInstance->HandleInput(EIA_Released, event.key.code);
		break;
	default:
		break;
	}
	return true;
}


void GameState::Draw()
{
	WindowManager::sInstance->draw(RenderManager::sInstance->GetBackgroundSprite());
	RenderManager::sInstance->RenderComponents();
}
