#pragma once


enum ECatControlType
{
	ESCT_Human,
	ESCT_AI
};

class RoboCatServer : public RoboCat
{
public:
	static GameObjectPtr	StaticCreate() { return NetworkManagerServer::sInstance->RegisterAndReturn(new RoboCatServer()); }
	virtual void HandleDying() override;

	virtual void Update() override;

	void SetCatControlType(ECatControlType inCatControlType) { mCatControlType = inCatControlType; }

	void TakeDamage(int inDamagingPlayerId, int inAmount);

	void AddHealth(int amount);
	void AddMachineGun(float duration, float newFireDelay);
	void AddInvincibility(float duration);

protected:
	RoboCatServer();

private:

	void HandleShooting();

	ECatControlType	mCatControlType;


	float		mTimeOfNextShot;
	float		mTimeBetweenShots;

	float mMachineGunTimer{ 0.f };
	float mOriginalTimeBetweenShots{ 0.2f };
};

