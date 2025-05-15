#include "RoboCatServerPCH.hpp"
#include <iostream>

#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#include <sys/stat.h>
#define MKDIR(path) mkdir(path, 0755)
#endif

extern const float WORLD_WIDTH = 1920.f;
extern const float WORLD_HEIGHT = 1080.f;

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
		Vector3 mouseMax(1720.f, 980.f, 0.f);

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

}

void Server::DoFrame()
{
	NetworkManagerServer::sInstance->ProcessIncomingPackets();

	NetworkManagerServer::sInstance->CheckForDisconnects();

	NetworkManagerServer::sInstance->RespawnCats();

	mGameTime += Timing::sInstance.GetDeltaTime();

	// 1) decrement round timer
	if (!mStopSpawningZombies)
	{
		mTimeRemaining -= Timing::sInstance.GetDeltaTime();
		if (mTimeRemaining <= 0.f)
		{
			mStopSpawningZombies = true;
			std::cout << "Round time up – stopping zombie spawns" << std::endl;
		}
	}

	if (!mStopSpawningZombies)
	{
		mZombieSpawnTimer -= Timing::sInstance.GetDeltaTime();
		if (mZombieSpawnTimer <= 0.f)
		{
			TrySpawnZombie();
			mNextZombieSpawnInterval = 1.f + RoboMath::GetRandomFloat() * (2.f - 1.f);
			mZombieSpawnTimer = mNextZombieSpawnInterval;
		}
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

	if (mStopSpawningZombies)
	{
		// scan world for any live zombies
		bool anyZombies = false;
		for (auto& go : World::sInstance->GetGameObjects())
		{
			if (go->GetClassId() == 'ZOMB' && !go->DoesWantToDie())
			{
				anyZombies = true;
				break;
			}
		}
		if (!anyZombies)
		{
			// broadcast game‐over to all clients
			EndGame();
		}
	}
}

void Server::EndGame()
{
	mGameEnded = true;
	NetworkManagerServer::sInstance->SendGameOverPacket();

	const char* dir = "Scores";
	if (MKDIR(dir) != 0 && errno != EEXIST)
	{
		std::cerr << "[Server] ERROR: failed to create directory '"
			<< dir << "' (errno=" << errno << ")\n";
	}

	auto now = std::chrono::system_clock::now();
	std::time_t t = std::chrono::system_clock::to_time_t(now);
	std::tm tm;
#ifdef _WIN32
	localtime_s(&tm, &t);
#else
	localtime_r(&t, &tm);
#endif

	std::ostringstream oss;
	oss << dir << "/scoreboard_"
		<< (tm.tm_year + 1900)
		<< std::setw(2) << std::setfill('0') << (tm.tm_mon + 1)
		<< std::setw(2) << std::setfill('0') << tm.tm_mday
		<< '_'
		<< std::setw(2) << std::setfill('0') << tm.tm_hour
		<< std::setw(2) << std::setfill('0') << tm.tm_min
		<< std::setw(2) << std::setfill('0') << tm.tm_sec
		<< ".csv";

	const std::string fileName = oss.str();

	// 3) write out the CSV
	if (ScoreBoardManager::sInstance->WriteToFile(fileName))
	{
		std::cout << "[Server] Wrote final scoreboard to "
			<< fileName << std::endl;
	}
	else
	{
		std::cerr << "[Server] ERROR: failed to write "
			<< fileName << std::endl;
	}
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
	auto zs = static_cast<ZombieServer*>(go.get());

	// random between 0 (default) and 1 (fast)
	auto t = (RoboMath::GetRandomFloat() < 0.3f  // e.g. 30% fast
		? Zombie::ZT_Fast
		: Zombie::ZT_Default);
	zs->SetType(t);
	go->SetLocation(spawn);
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
