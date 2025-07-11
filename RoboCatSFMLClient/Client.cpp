/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

#include "RoboCatClientPCH.hpp"

bool Client::StaticInit()
{
	Client* client = new Client();


	WindowManager::StaticInit();
	FontManager::StaticInit();
	TextureManager::StaticInit();
	RenderManager::StaticInit();
	StackManager::StaticInit();
	HUD::StaticInit();
	AudioManager::StaticInit();

	s_instance.reset(client);
	return true;
}

Client::Client()
{
	// Register all client-side GameObjects
	GameObjectRegistry::sInstance->RegisterCreationFunction('RCAT', RoboCatClient::StaticCreate);
	GameObjectRegistry::sInstance->RegisterCreationFunction('MOUS', MouseClient::StaticCreate);
	GameObjectRegistry::sInstance->RegisterCreationFunction('YARN', YarnClient::StaticCreate);
	GameObjectRegistry::sInstance->RegisterCreationFunction('ZOMB', ZombieClient::StaticCreate);


}

void Client::DoFrame()
{
	StackManager::sInstance->Update(Timing::sInstance.GetDeltaTime());

	AudioManager::sInstance->RemoveStoppedSounds();

	if (StackManager::sInstance->IsEmpty())
	{
		Engine::s_instance->SetShouldKeepRunning(false);
		WindowManager::sInstance->close();
		return;
	}

	RenderManager::sInstance->Render();
}

void Client::HandleEvent(sf::Event& p_event)
{
	StackManager::sInstance->HandleEvent(p_event);

	if (p_event.type == sf::Event::Closed)
	{
		WindowManager::sInstance->close();
	}
}

bool Client::PollEvent(sf::Event& p_event)
{
	return WindowManager::sInstance->pollEvent(p_event);
}

