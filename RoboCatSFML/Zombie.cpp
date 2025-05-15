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
    //Assigning correct HP by zombie type
    mHealth = (mType == ZT_Fast ? 1 : 4);
}

static constexpr float kZombieMaxSpeed = 100.f;

void Zombie::Update()
{

}

void Zombie::SimulateMovement(float dt)
{
	//Chooses the speed based on the zombie type
    float speed = (mType == ZT_Fast ? kFastSpeed : kDefaultSpeed);
    mVelocity = mMovementDirection * speed;
    SetLocation(GetLocation() + mVelocity * dt);

    //Rotates to face the nearest player
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

    dirty = (inDirtyState & ZRS_Pose) != 0;
    out.Write(dirty);
    if (dirty)
    {
        out.Write(mVelocity.mX);
        out.Write(mVelocity.mY);

        Vector3 loc = GetLocation();
        out.Write(loc.mX);
        out.Write(loc.mY);

        out.Write(GetRotation());

        out.Write(mMovementDirection.mX);
        out.Write(mMovementDirection.mY);

        written |= ZRS_Pose;
    }

    dirty = (inDirtyState & ZRS_Health) != 0;
    out.Write(dirty);
    if (dirty)
    {
        out.Write(mHealth, 5);
        written |= ZRS_Health;
    }

    dirty = (inDirtyState & ZRS_Type) != 0;
    out.Write(dirty);
    if (dirty)
    {
        out.Write(mType, 5);
        written |= ZRS_Type;
    }

    return written;
}
