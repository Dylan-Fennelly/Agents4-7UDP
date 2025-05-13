#include "RoboCatPCH.hpp"

Zombie::Zombie()
    : GameObject()
    , mVelocity(Vector3::Zero)
    , mMovementDirection(Vector3::Zero)
    , mHealth(100)
{
}

static constexpr float kZombieMaxSpeed = 100.f;

void Zombie::Update()
{

}

void Zombie::SimulateMovement(float inDeltaTime)
{
    // 1) velocity snaps to direction × max speed
    mVelocity = mMovementDirection * kZombieMaxSpeed;

    // 2) advance the position
    Vector3 newLoc = GetLocation() + mVelocity * inDeltaTime;
    SetLocation(newLoc);

    // 3) (optional) update rotation so the sprite faces movement
    if (mMovementDirection.LengthSq2D() > 0.f)
    {
        float angle = RoboMath::ToDegrees(
            atan2f(mMovementDirection.mX,
                -mMovementDirection.mY)
        );
        SetRotation(angle);
    }

    // 4) (optional) handle collisions if you want zombies to bounce
    //    ProcessCollisions();
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

    return written;
}
