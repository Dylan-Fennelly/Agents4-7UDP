/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

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

		//Logic for checking if the yarn is within the zombie's radius
        Vector3 delta = zs->GetLocation() - myLoc;
        float   distSq = delta.LengthSq2D();
        float   hitDist = myRadius + zs->GetCollisionRadius();

        if (distSq < hitDist * hitDist)
        {
            //Zombie gets a hit
            int newHP = zs->GetHealth() - kYarnDamage;
            zs->SetHealth(newHP);

            NetworkManagerServer::sInstance->SetStateDirty(
                zs->GetNetworkId(),
                Zombie::ZRS_Health
            );

            //zombie = dead
            if (newHP <= 0)
            {
                zs->SetDoesWantToDie(true);
                NetworkManagerServer::sInstance->SetStateDirty(
                    zs->GetNetworkId(),
                    Zombie::ZRS_AllState
                );
                ScoreBoardManager::sInstance->IncScore(GetPlayerId(), 1);
            }

            //bye bye yarn
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



