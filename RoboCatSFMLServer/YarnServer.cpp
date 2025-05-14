#include "RoboCatServerPCH.hpp"

static constexpr int kYarnDamage = 1;

YarnServer::YarnServer()
{
	//yarn lives 5 seconds...
	mTimeToDie = Timing::sInstance.GetFrameStartTime() + 5.f;
}

void YarnServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject(this);
}


void YarnServer::Update()
{
	Yarn::Update();

    // 2) Check collision vs. any live ZombieServer
    float    myRadius = GetCollisionRadius();
    Vector3  myLoc = GetLocation();

    for (auto& goPtr : World::sInstance->GetGameObjects())
    {
        auto zs = dynamic_cast<ZombieServer*>(goPtr.get());
        if (!zs || zs->DoesWantToDie())
            continue;

        Vector3 delta = zs->GetLocation() - myLoc;
        float   distSq = delta.LengthSq2D();
        float   hitDist = myRadius + zs->GetCollisionRadius();

        if (distSq < hitDist * hitDist)
        {
            // ——— damage the zombie ———
            int newHP = zs->GetHealth() - kYarnDamage;
            zs->SetHealth(newHP);

            // replicate just the health change
            NetworkManagerServer::sInstance->SetStateDirty(
                zs->GetNetworkId(),
                Zombie::ZRS_Health
            );

            // if it’s dead, flag for removal (and replicate all state so client erases it)
            if (newHP <= 0)
            {
                zs->SetDoesWantToDie(true);
                NetworkManagerServer::sInstance->SetStateDirty(
                    zs->GetNetworkId(),
                    Zombie::ZRS_AllState
                );
                ScoreBoardManager::sInstance->IncScore(GetPlayerId(), 1);
            }

            // ——— destroy this yarn ———
            SetDoesWantToDie(true);
            break;
        }
    }

	if (Timing::sInstance.GetFrameStartTime() > mTimeToDie)
	{
		SetDoesWantToDie(true);
	}

}

bool YarnServer::HandleCollisionWithCat(RoboCat* inCat)
{
	if (inCat->GetPlayerId() != GetPlayerId())
	{
		//kill yourself!
		SetDoesWantToDie(true);

		static_cast<RoboCatServer*>(inCat)->TakeDamage(GetPlayerId(), 1);

	}

	return false;
}



