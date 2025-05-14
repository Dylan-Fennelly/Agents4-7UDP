#include "RoboCatServerPCH.hpp"


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
    // snapshot old state
    Vector3 oldLoc = GetLocation(), oldVel = GetVelocity();
    float   oldRot = GetRotation();
    int     oldHP = GetHealth();

    // → run your AI & movement logic here
    SimulateAI();
    SimulateMovement(Timing::sInstance.GetDeltaTime());

    ProcessCollisionsWithCats();

    // if anything changed, flag for replication
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
    // scan all game objects for the closest RoboCat
    GameObjectPtr  bestGo;
    RoboCat* bestCat = nullptr;
    float          bestDistSq = FLT_MAX;

    for (auto& goPtr : World::sInstance->GetGameObjects())
    {
        // is it a live player‐cat?
        if (auto cat = goPtr->GetAsCat())
        {
            if (!cat->DoesWantToDie())  // skip “dead” cats
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
        // point at them
        Vector3 toPlayer = bestCat->GetLocation() - GetLocation();
        toPlayer.Normalize2D();
        SetMovementDirection(toPlayer);
    }
    else
    {
        // no players alive: stand still
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

        // Only care about player cats
        if (auto cat = goPtr->GetAsCat())
        {
            // Skip if the cat is already dead
            if (cat->DoesWantToDie())
                continue;

            Vector3 delta = cat->GetLocation() - myLoc;
            float distSq = delta.LengthSq2D();
            float collisionDist = myRadius + cat->GetCollisionRadius();

            if (distSq < (collisionDist * collisionDist))
            {
                auto catServer = static_cast<RoboCatServer*>(cat);

                // This will skip damage if invincibility is active
                catServer->TakeDamage(0);

                // zombie self‐destructs on contact
                SetDoesWantToDie(true);
                ScoreBoardManager::sInstance->IncScore(cat->GetPlayerId(), 1);
                break;
            }
        }
    }
}
