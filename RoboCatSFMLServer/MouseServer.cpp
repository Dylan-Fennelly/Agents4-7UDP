#include "RoboCatServerPCH.hpp"

MouseServer::MouseServer()
{
}

void MouseServer::HandleDying()
{
	NetworkManagerServer::sInstance->UnregisterGameObject(this);
}


bool MouseServer::HandleCollisionWithCat(RoboCat* inCat)
{
    auto* serverCat = static_cast<RoboCatServer*>(inCat);

    switch (GetType())
    {
    case Type::Health:
        serverCat->AddHealth(5);               // give 5 health
        break;

    case Type::MachineGun:
        serverCat->AddMachineGun(10.f, 0.1f);  // 10 s at 10× fire rate
        break;

    case Type::Invincibility:
        serverCat->AddInvincibility(5.f);      // 5 s invulnerable
        break;
    }

	//kill yourself!
	SetDoesWantToDie(true);

	//ScoreBoardManager::sInstance->IncScore(inCat->GetPlayerId(), 1);

	return false;
}