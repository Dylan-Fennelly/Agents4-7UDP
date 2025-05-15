#include "RoboCatClientPCH.hpp"

ZombieClient::ZombieClient()
    : mTimeLocationBecameOutOfSync(0.f)
    , mTimeVelocityBecameOutOfSync(0.f)
{
    mSpriteComponent.reset(new SpriteComponent(this));
    mSpriteComponent->SetTexture(
        TextureManager::sInstance->GetTexture("zombie")
    );
}

void ZombieClient::HandleDying()
{
    GameObject::HandleDying();
    AudioManager::sInstance->Play("zombie");
}
void ZombieClient::Update()
{
    // always simulate remote‐driven movement
    SimulateMovement(Timing::sInstance.GetDeltaTime());
}

void ZombieClient::Read(InputMemoryBitStream& inInputStream)
{
    bool stateBit;
    uint32_t readState = 0;

    GameObject::Read(inInputStream);

    float   oldRot = GetRotation();
    Vector3 oldLoc = GetLocation();
    Vector3 oldVel = GetVelocity();

    // ——— Pose ———
    inInputStream.Read(stateBit);
    if (stateBit)
    {
        Vector3 vel, loc, dir;
        float   rot;

        inInputStream.Read(vel.mX); 
        inInputStream.Read(vel.mY);       
        SetVelocity(vel);

        inInputStream.Read(loc.mX); 
        inInputStream.Read(loc.mY);       
        SetLocation(loc);

        inInputStream.Read(rot); 
        SetRotation(rot);

        inInputStream.Read(dir.mX); 
        inInputStream.Read(dir.mY);       
        SetMovementDirection(dir);

        readState |= ZRS_Pose;
    }

    // ——— Health ———
    inInputStream.Read(stateBit);
    if (stateBit)
    {
        int h = 0;
        inInputStream.Read(h, 5);  // 5 bits for health 0–31
        SetHealth(h);
        readState |= ZRS_Health;
    }

    inInputStream.Read(stateBit);
    if (stateBit)
    {
        uint8_t t;
        inInputStream.Read(t, 5);
        SetType(static_cast<Zombie::EZombieType>(t));
        readState |= ZRS_Type;
    }
    
    const char* key = (GetType() == Zombie::ZT_Fast)
        ? "fastZombie"
        : "zombie";
    mSpriteComponent->SetTexture(
        TextureManager::sInstance->GetTexture(key)
    );

    // client‐side prediction for remote zombies
    DoClientSidePredictionAfterReplicationForRemoteZombie(readState);

    if ((readState & ZRS_Pose) == 0)
    {
        InterpolateClientSidePrediction(oldRot, oldLoc, oldVel);
    }
}

void ZombieClient::InterpolateClientSidePrediction(
    float inOldRotation,
    const Vector3& inOldLocation,
    const Vector3& inOldVelocity)
{
    // identical to RoboCatClient::InterpolateClientSidePrediction,
    // but we always treat it as “remote” interpolation :contentReference[oaicite:0]{index=0}:contentReference[oaicite:1]{index=1}

    float roundTripTime = NetworkManagerClient::sInstance->GetRoundTripTime();

    // Location smoothing
    if (!RoboMath::Is2DVectorEqual(inOldLocation, GetLocation()))
    {
        float time = Timing::sInstance.GetFrameStartTime();
        if (mTimeLocationBecameOutOfSync == 0.f)
            mTimeLocationBecameOutOfSync = time;

        float durationOutOfSync = time - mTimeLocationBecameOutOfSync;
        if (durationOutOfSync < roundTripTime)
        {
            float t = durationOutOfSync / roundTripTime;
            SetLocation(Lerp(inOldLocation, GetLocation(), t));
        }
    }
    else
    {
        mTimeLocationBecameOutOfSync = 0.f;
    }

    // Velocity smoothing
    if (!RoboMath::Is2DVectorEqual(inOldVelocity, GetVelocity()))
    {
        float time = Timing::sInstance.GetFrameStartTime();
        if (mTimeVelocityBecameOutOfSync == 0.f)
            mTimeVelocityBecameOutOfSync = time;

        float durationOutOfSync = time - mTimeVelocityBecameOutOfSync;
        if (durationOutOfSync < roundTripTime)
        {
            float t = durationOutOfSync / roundTripTime;
            SetVelocity(Lerp(inOldVelocity, GetVelocity(), t));
        }
    }
    else
    {
        mTimeVelocityBecameOutOfSync = 0.f;
    }
}

void ZombieClient::DoClientSidePredictionAfterReplicationForRemoteZombie(uint32_t inReadState)
{
    // mirror RoboCatClient’s extra‐RTT simulation :contentReference[oaicite:2]{index=2}:contentReference[oaicite:3]{index=3}
    if (inReadState & ZRS_Pose)
    {
        float rtt = NetworkManagerClient::sInstance->GetRoundTripTime();
        const float deltaTime = 1.f / 30.f;

        // step‐through in frame‐sized chunks
        while (rtt > deltaTime)
        {
            SimulateMovement(deltaTime);
            rtt -= deltaTime;
        }
        SimulateMovement(rtt);
    }
}