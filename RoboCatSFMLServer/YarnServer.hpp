/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

class YarnServer : public Yarn
{
public:
	static GameObjectPtr	StaticCreate() { return NetworkManagerServer::sInstance->RegisterAndReturn(new YarnServer()); }
	void HandleDying() override;

	virtual bool		HandleCollisionWithCat(RoboCat* inCat) override;

	virtual void Update() override;

protected:
	YarnServer();

private:
	float mTimeToDie;

};

