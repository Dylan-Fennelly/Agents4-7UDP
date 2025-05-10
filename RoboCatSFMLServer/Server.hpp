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

	float mMouseSpawnTimer = 0.0f; // Tracks elapsed time for spawning mice
	const float mMouseSpawnInterval = 10.0f; // Interval in seconds to spawn a mouse
};

