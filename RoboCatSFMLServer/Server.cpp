#include "RoboCatServerPCH.hpp"
#include <iostream>

extern const float WORLD_WIDTH = 1280.f;
extern const float WORLD_HEIGHT = 720.f;

//uncomment this when you begin working on the server

bool Server::StaticInit()
{
	s_instance.reset(new Server());

	return true;
}

Server::Server()
{

	GameObjectRegistry::sInstance->RegisterCreationFunction('RCAT', RoboCatServer::StaticCreate);
	GameObjectRegistry::sInstance->RegisterCreationFunction('MOUS', MouseServer::StaticCreate);
	GameObjectRegistry::sInstance->RegisterCreationFunction('YARN', YarnServer::StaticCreate);
	GameObjectRegistry::sInstance->RegisterCreationFunction('ZOMB', ZombieServer::StaticCreate);

	InitNetworkManager();

	// Setup latency
	float latency = 0.0f;
	string latencyString = StringUtils::GetCommandLineArg(2);
	if (!latencyString.empty())
	{
		latency = stof(latencyString);
	}
	NetworkManagerServer::sInstance->SetSimulatedLatency(latency);

	mNextZombieSpawnInterval = 1.f + RoboMath::GetRandomFloat() * (2.f - 1.f);
	mZombieSpawnTimer = mNextZombieSpawnInterval;
}


int Server::Run()
{
	SetupWorld();

	return Engine::Run();
}

bool Server::InitNetworkManager()
{
	string portString = StringUtils::GetCommandLineArg(1);
	uint16_t port = stoi(portString);

	return NetworkManagerServer::StaticInit(port);
}


namespace
{

	void CreateRandomMice(int inMouseCount)
	{
		Vector3 mouseMin(100.f, 100.f, 0.f);
		Vector3 mouseMax(1180.f, 620.f, 0.f);

		for (int i = 0; i < inMouseCount; ++i)
		{
			GameObjectPtr go = GameObjectRegistry::sInstance->CreateGameObject('MOUS');
			Vector3 mouseLocation = RoboMath::GetRandomVector(mouseMin, mouseMax);
			go->SetLocation(mouseLocation);

			// assign random pickup type
			auto mouseServer = static_cast<MouseServer*>(go.get());
			int r = RoboMath::GetRandomInt(0, 2);
			mouseServer->SetType(static_cast<Mouse::Type>(r));
		}
	}
}

void Server::SetupWorld()
{
	//auto go = GameObjectRegistry::sInstance->CreateGameObject('ZOMB');
	//go->SetLocation(Vector3(600, 500, 0.f));
}

void Server::DoFrame()
{
	NetworkManagerServer::sInstance->ProcessIncomingPackets();

	NetworkManagerServer::sInstance->CheckForDisconnects();

	NetworkManagerServer::sInstance->RespawnCats();

	mZombieSpawnTimer -= Timing::sInstance.GetDeltaTime();
    if (mZombieSpawnTimer <= 0.f)
    {
        TrySpawnZombie();
        // pick a new interval and reset timer
        mNextZombieSpawnInterval = 1.f + RoboMath::GetRandomFloat() * (2.f - 1.f);
        mZombieSpawnTimer        = mNextZombieSpawnInterval;
    }

	// Update the mouse spawn timer
	mMouseSpawnTimer += Timing::sInstance.GetDeltaTime();
	if (mMouseSpawnTimer >= mMouseSpawnInterval)
	{
		// Spawn one mouse
		CreateRandomMice(1);
		mMouseSpawnTimer = 0.0f; // Reset the timer
	}

	Engine::DoFrame();

	NetworkManagerServer::sInstance->SendOutgoingPackets();

}

void Server::TrySpawnZombie()
{
	const float margin = 50.f;
	int side = RoboMath::GetRandomInt(0, 3);
	Vector3 spawn;

	switch (side)
	{
	case 0: // top
		spawn.mX = RoboMath::GetRandomFloat() * WORLD_WIDTH;
		spawn.mY = WORLD_HEIGHT + margin;
		break;
	case 1: // bottom
		spawn.mX = RoboMath::GetRandomFloat() * WORLD_WIDTH;
		spawn.mY = -margin;
		break;
	case 2: // left
		spawn.mX = -margin;
		spawn.mY = RoboMath::GetRandomFloat() * WORLD_HEIGHT;
		break;
	default: // right
		spawn.mX = WORLD_WIDTH + margin;
		spawn.mY = RoboMath::GetRandomFloat() * WORLD_HEIGHT;
		break;
	}

	auto go = GameObjectRegistry::sInstance->CreateGameObject('ZOMB');
	go->SetLocation(spawn);
	//auto zs = static_cast<ZombieServer*>(go.get());
	//zs->SetCatControlType(ESCT_AI);
}

void Server::HandleNewClient(ClientProxyPtr inClientProxy)
{

	int playerId = inClientProxy->GetPlayerId();

	ScoreBoardManager::sInstance->AddEntry(playerId, inClientProxy->GetName());
	SpawnCatForPlayer(playerId);
}

void Server::SpawnCatForPlayer(int inPlayerId)
{
	RoboCatPtr cat = std::static_pointer_cast<RoboCat>(GameObjectRegistry::sInstance->CreateGameObject('RCAT'));
	//cat->SetColor(ScoreBoardManager::sInstance->GetEntry(inPlayerId)->GetColor());
	cat->SetPlayerId(inPlayerId);
	//gotta pick a better spawn location than this...
	cat->SetLocation(Vector3(600.f - static_cast<float>(inPlayerId), 400.f, 0.f));
}

void Server::HandleLostClient(ClientProxyPtr inClientProxy)
{
	//kill client's cat
	//remove client from scoreboard
	int playerId = inClientProxy->GetPlayerId();

	ScoreBoardManager::sInstance->RemoveEntry(playerId);
	RoboCatPtr cat = GetCatForPlayer(playerId);
	if (cat)
	{
		cat->SetDoesWantToDie(true);
	}
}

RoboCatPtr Server::GetCatForPlayer(int inPlayerId)
{
	//run through the objects till we find the cat...
	//it would be nice if we kept a pointer to the cat on the clientproxy
	//but then we'd have to clean it up when the cat died, etc.
	//this will work for now until it's a perf issue
	const auto& gameObjects = World::sInstance->GetGameObjects();
	for (int i = 0, c = gameObjects.size(); i < c; ++i)
	{
		GameObjectPtr go = gameObjects[i];
		RoboCat* cat = go->GetAsCat();
		if (cat && cat->GetPlayerId() == inPlayerId)
		{
			return std::static_pointer_cast<RoboCat>(go);
		}
	}

	return nullptr;
}
