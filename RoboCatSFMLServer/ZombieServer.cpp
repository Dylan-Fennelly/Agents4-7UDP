/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

#include "RoboCatServerPCH.hpp"

static constexpr int kDefaultZombieDamage = 2;
static constexpr int kFastZombieDamage = 5;

ZombieServer::ZombieServer() : Zombie() 
{
    SetCollisionRadius(10.f);
}
void ZombieServer::HandleDying()
{
    NetworkManagerServer::sInstance->UnregisterGameObject(this);
}

void ZombieServer::Update()
{
    Vector3 oldLoc = GetLocation(), oldVel = GetVelocity();
    float   oldRot = GetRotation();
    int     oldHP = GetHealth();

    SimulateAI();
    SimulateMovement(Timing::sInstance.GetDeltaTime());

    ProcessCollisionsWithCats();

    if (!RoboMath::Is2DVectorEqual(oldLoc, GetLocation()) ||
        !RoboMath::Is2DVectorEqual(oldVel, GetVelocity()) ||
        oldRot != GetRotation() ||
        oldHP != GetHealth())
    {
        NetworkManagerServer::sInstance->SetStateDirty(
            GetNetworkId(), ZRS_AllState);
    }
}

void ZombieServer::SimulateAI()
{
    //Looks for the closest player
    GameObjectPtr  bestGo;
    RoboCat* bestCat = nullptr;
    float          bestDistSq = FLT_MAX;

    for (auto& goPtr : World::sInstance->GetGameObjects())
    {
        if (auto cat = goPtr->GetAsCat())
        {
            if (!cat->DoesWantToDie())
            {
                float d2 = (cat->GetLocation() - GetLocation()).LengthSq2D();
                if (d2 < bestDistSq)
                {
                    bestDistSq = d2;
                    bestCat = cat;
                }
            }
        }
    }

    if (bestCat)
    {
        Vector3 toPlayer = bestCat->GetLocation() - GetLocation();
        toPlayer.Normalize2D();
        SetMovementDirection(toPlayer);
    }
    else
    {
		//Stand still, I mean, where would you even go?
        SetMovementDirection(Vector3::Zero);
    }
}

void ZombieServer::ProcessCollisionsWithCats()
{
    float myRadius = GetCollisionRadius();
    Vector3 myLoc = GetLocation();

    for (auto& goPtr : World::sInstance->GetGameObjects())
    {
        if (goPtr.get() == this)
            continue;

        if (auto cat = goPtr->GetAsCat())
        {
            if (cat->DoesWantToDie())
                continue;

            Vector3 delta = cat->GetLocation() - myLoc;
            float distSq = delta.LengthSq2D();
            float collisionDist = myRadius + cat->GetCollisionRadius();

            if (distSq < (collisionDist * collisionDist))
            {
                auto catServer = static_cast<RoboCatServer*>(cat);

                //Damage
                int dmg = (GetType() == Zombie::ZT_Fast
                    ? kFastZombieDamage
                    : kDefaultZombieDamage);
				//Had to re-write TakeDamage to work with passing in different damage values
                catServer->TakeDamage(0, dmg);
                SetDoesWantToDie(true);
                ScoreBoardManager::sInstance->IncScore(cat->GetPlayerId(), 1);
                break;
            }
        }
    }
}
