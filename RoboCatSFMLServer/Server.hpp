class Server : public Engine
{
public:

	static bool StaticInit();

	virtual void DoFrame() override;

	virtual int Run();

	void HandleNewClient(ClientProxyPtr inClientProxy);
	void HandleLostClient(ClientProxyPtr inClientProxy);

	RoboCatPtr	GetCatForPlayer(int inPlayerId);
	void	SpawnCatForPlayer(int inPlayerId);


private:
	Server();

	bool	InitNetworkManager();
	void	SetupWorld();
	void TrySpawnZombie();

	void EndGame();

	float mMouseSpawnTimer = 0.0f; // Tracks elapsed time for spawning mice
	const float mMouseSpawnInterval = 10.0f; // Interval in seconds to spawn a mouse

	float mZombieSpawnTimer = 0.f;
	float mNextZombieSpawnInterval = 0.f;

	float   mGameTime = 0.f;         // seconds since match start
	bool    mGameEnded = false;       // guard so we only end once
	static constexpr float kMaxGameTime = 15.f;
};

