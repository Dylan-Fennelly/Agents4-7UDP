/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

class ZombieServer : public Zombie
{
public:
    static GameObjectPtr StaticCreate()
    {
        return NetworkManagerServer::sInstance->RegisterAndReturn(new ZombieServer());
    }

    virtual void Update() override;
    virtual void HandleDying() override;
protected:
    ZombieServer();
    void SimulateAI();
	void ProcessCollisionsWithCats();
};

