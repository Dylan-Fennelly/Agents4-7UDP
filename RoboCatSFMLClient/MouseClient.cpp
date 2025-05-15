/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

#include "RoboCatClientPCH.hpp"

MouseClient::MouseClient()
{
	mSpriteComponent.reset(new SpriteComponent(this));
}

void MouseClient::Read(InputMemoryBitStream& inInputStream)
{
    Mouse::Read(inInputStream);

    //Read the type
    uint8_t typeVal = 0;
    inInputStream.Read(typeVal);
    Type t = static_cast<Type>(typeVal);
    SetType(t);
    AudioManager::sInstance->Play("pickup");
    //Pick the right texture
    switch (t)
    {
    case Type::Health:
        mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("health"));
        break;
    case Type::MachineGun:
        mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("machinegun"));
        break;
    case Type::Invincibility:
        mSpriteComponent->SetTexture(TextureManager::sInstance->GetTexture("invincibility"));
        break;
    }
}