#include "RoboCatClientPCH.hpp"

MouseClient::MouseClient()
{
	mSpriteComponent.reset(new SpriteComponent(this));
}

void MouseClient::Read(InputMemoryBitStream& inInputStream)
{
    // 1) read pose & color
    Mouse::Read(inInputStream);

    // 2) now read our Type (matches write order)
    uint8_t typeVal = 0;
    inInputStream.Read(typeVal);
    Type t = static_cast<Type>(typeVal);
    SetType(t);
    AudioManager::sInstance->Play("pickup");
    // 3) pick the right texture
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