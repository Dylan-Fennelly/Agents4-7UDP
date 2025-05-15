#include "RoboCatClientPCH.hpp"

GameState::GameState(StateStack& stack):State(stack)
{
	InputManager::StaticInit();
}

bool GameState::Update(float dt)
{
	if (!m_networkInitialised)
	{
		std::ifstream ipFile("ip.txt");
		std::ifstream portFile("port.txt");
		std::ifstream nameFile("username.txt");

		std::string ip = "127.0.0.1";      // default fallback IP
		std::string port = "50000";        // default fallback port
		std::string name = "Player";       // default fallback name

		if (ipFile >> ip) {}
		else { std::ofstream("ip.txt") << ip; }

		if (portFile >> port) {}
		else { std::ofstream("port.txt") << port; }

		if (nameFile >> name) {}
		else { std::ofstream("username.txt") << name; }

		std::string fullAddress = ip + ":" + port;
		SocketAddressPtr serverAddress = SocketAddressFactory::CreateIPv4FromString(fullAddress);
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
	HUD::sInstance->Render();
}

void GameState::OnStackPopped()
{
	//we need to handle Disconnecting from the game 
	m_networkInitialised = false;
}

