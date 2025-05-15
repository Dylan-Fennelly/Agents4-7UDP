/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

#include "RoboCatServerPCH.hpp"

RoboCatServer::RoboCatServer() :
	mCatControlType(ESCT_Human),
	mTimeOfNextShot(0.f),
	mTimeBetweenShots(1.2f)
{}

void RoboCatServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject(this);
}

void RoboCatServer::AddHealth(int amount)
{
	mHealth = std::min(mHealth + amount, mMaxHealth);
	NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), ECRS_Health);
}

void RoboCatServer::AddMachineGun(float duration, float newFireDelay)
{
	if (mMachineGunTimer <= 0.f)
		mOriginalTimeBetweenShots = mTimeBetweenShots;

	mTimeBetweenShots = newFireDelay;
	mMachineGunTimer = duration;
	NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), ECRS_MachineGunTimer);
}

void RoboCatServer::AddInvincibility(float duration)
{
	mInvincibilityTimer = duration;
	NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), ECRS_InvincibilityTimer);
}

void RoboCatServer::Update()
{
	RoboCat::Update();

	

	Vector3 oldLocation = GetLocation();
	Vector3 oldVelocity = GetVelocity();
	float oldRotation = GetRotation();

	float dt = Timing::sInstance.GetDeltaTime();

	if (mInvincibilityTimer > 0.f)
	{
		mInvincibilityTimer -= dt;
		NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), ECRS_InvincibilityTimer);
	}

	if (mMachineGunTimer > 0.f)
	{
		mMachineGunTimer -= dt;
		if (mMachineGunTimer <= 0.f)
			mTimeBetweenShots = mOriginalTimeBetweenShots;
	}

	//are you controlled by a player?
	//if so, is there a move we haven't processed yet?
	if (mCatControlType == ESCT_Human)
	{
		ClientProxyPtr client = NetworkManagerServer::sInstance->GetClientProxy(GetPlayerId());
		if (client)
		{
			MoveList& moveList = client->GetUnprocessedMoveList();
			for (const Move& unprocessedMove : moveList)
			{
				const InputState& currentState = unprocessedMove.GetInputState();

				float deltaTime = unprocessedMove.GetDeltaTime();

				ProcessInput(deltaTime, currentState);
				SimulateMovement(deltaTime);
			}

			moveList.Clear();
		}
	}
	else
	{
		//do some AI stuff
		SimulateMovement(Timing::sInstance.GetDeltaTime());
	}


	HandleShooting();

	if (!RoboMath::Is2DVectorEqual(oldLocation, GetLocation()) ||
		!RoboMath::Is2DVectorEqual(oldVelocity, GetVelocity()) ||
		oldRotation != GetRotation())
	{
		NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), ECRS_Pose);
	}
}

void RoboCatServer::HandleShooting()
{
	float time = Timing::sInstance.GetFrameStartTime();
	if (mIsShooting && Timing::sInstance.GetFrameStartTime() > mTimeOfNextShot)
	{
		//not exact, but okay
		mTimeOfNextShot = time + mTimeBetweenShots;

		//fire!
		YarnPtr yarn = std::static_pointer_cast<Yarn>(GameObjectRegistry::sInstance->CreateGameObject('YARN'));
		yarn->InitFromShooter(this);
	}
}

void RoboCatServer::TakeDamage(int inDamagingPlayerId, int inAmount)
{
	if (mInvincibilityTimer > 0.f)
		return;

	mHealth -= inAmount;
	if (mHealth <= 0.f)
	{
		ScoreBoardManager::sInstance->SetDeath(GetPlayerId());

		//and you want to die
		SetDoesWantToDie(true);

		//tell the client proxy to make you a new cat
		ClientProxyPtr clientProxy = NetworkManagerServer::sInstance->GetClientProxy(GetPlayerId());
		if (clientProxy)
		{
			clientProxy->HandleCatDied();
		}
	}

	//tell the world our health dropped
	NetworkManagerServer::sInstance->SetStateDirty(GetNetworkId(), ECRS_Health);
}

