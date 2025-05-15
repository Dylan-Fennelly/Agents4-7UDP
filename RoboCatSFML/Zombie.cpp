/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

#include "RoboCatPCH.hpp"

static constexpr float kDefaultSpeed = 80.f;
static constexpr float kFastSpeed = 250.f;

Zombie::Zombie()
    : GameObject()
    , mVelocity(Vector3::Zero)
    , mMovementDirection(Vector3::Zero)
{
}

void Zombie::SetType(EZombieType t)
{
    mType = t;
    // now that we know the type, assign the correct HP:
    mHealth = (mType == ZT_Fast ? 1 : 4);
}

static constexpr float kZombieMaxSpeed = 100.f;

void Zombie::Update()
{

}

void Zombie::SimulateMovement(float dt)
{
    // pick speed by type
    float speed = (mType == ZT_Fast ? kFastSpeed : kDefaultSpeed);
    mVelocity = mMovementDirection * speed;
    SetLocation(GetLocation() + mVelocity * dt);

    // rotate to face
    if (mMovementDirection.LengthSq2D() > 0.f)
    {
        float angle = RoboMath::ToDegrees(atan2f(
            mMovementDirection.mX, -mMovementDirection.mY));
        SetRotation(angle);
    }
}

uint32_t Zombie::Write(OutputMemoryBitStream& out, uint32_t inDirtyState) const
{
    uint32_t written = 0;
    bool     dirty = false;

    // ——— Pose ———
    dirty = (inDirtyState & ZRS_Pose) != 0;
    out.Write(dirty);
    if (dirty)
    {
        // velocity
        out.Write(mVelocity.mX);
        out.Write(mVelocity.mY);

        // position
        Vector3 loc = GetLocation();
        out.Write(loc.mX);
        out.Write(loc.mY);

        // rotation
        out.Write(GetRotation());

        // movement direction
        out.Write(mMovementDirection.mX);
        out.Write(mMovementDirection.mY);

        written |= ZRS_Pose;
    }

    // ——— Health ———
    dirty = (inDirtyState & ZRS_Health) != 0;
    out.Write(dirty);
    if (dirty)
    {
        // pack health into 5 bits (0–31)
        out.Write(mHealth, 5);
        written |= ZRS_Health;
    }

    // Type
    dirty = (inDirtyState & ZRS_Type) != 0;
    out.Write(dirty);
    if (dirty)
    {
        out.Write(mType, 5);
        written |= ZRS_Type;
    }

    return written;
}
