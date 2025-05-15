#include "RoboCatPCH.hpp"

const float WORLD_HEIGHT = 1080.f;
const float WORLD_WIDTH = 1920.f;

RoboCat::RoboCat() :
	GameObject(),
	mMaxRotationSpeed(500.f),
	mMaxLinearSpeed(200.f),
	mVelocity(Vector3::Zero),
	mWallRestitution(0.1f),
	mCatRestitution(0.1f),
	mThrustDir(0.f),
	mPlayerId(0),
	mIsShooting(false),
	mHealth(10),
	mInvincibilityTimer(0.f)
{
	SetCollisionRadius(10.f);
}

void RoboCat::ProcessInput(float inDeltaTime, const InputState& inInputState)
{
	float currentRot = GetRotation();
	float targetRot = inInputState.GetDesiredRotation();

	// Normalise angles to [0, 360)
	currentRot = fmodf(currentRot + 360.f, 360.f);
	targetRot = fmodf(targetRot + 360.f, 360.f);

	// Get shortest angle delta in [-180, 180]
	float delta = fmodf(targetRot - currentRot + 540.f, 360.f) - 180.f;

	// Clamp based on max speed
	float maxRotDelta = mMaxRotationSpeed * inDeltaTime;
	delta = RoboMath::Clamp(delta, -maxRotDelta, maxRotDelta);

	// Apply delta
	SetRotation(currentRot + delta);

	// Movement input
	mInputDirection = Vector3(-inInputState.GetDesiredHorizontalDelta(), -inInputState.GetDesiredVerticalDelta(), 0.f);
	if (mInputDirection.LengthSq2D() > 1.f)
	{
		mInputDirection.Normalize2D();
	}

	mIsShooting = inInputState.IsShooting();
}


void RoboCat::AdjustVelocityByThrust(float inDeltaTime)
{
	if (mInputDirection.LengthSq2D() > 0.f)
	{
		// Snap velocity instantly to max speed in the input direction
		mVelocity = mInputDirection * mMaxLinearSpeed;
	}
	else
	{
		// No input: stop movement completely or add a small decay if you want it to coast slightly
		mVelocity = Vector3::Zero;
	}
}

void RoboCat::SimulateMovement(float inDeltaTime)
{
	//simulate us...
	AdjustVelocityByThrust(inDeltaTime);
	
	SetLocation(GetLocation() + mVelocity * inDeltaTime);

	ProcessCollisions();
}

void RoboCat::Update()
{

}

void RoboCat::ProcessCollisions()
{
	//right now just bounce off the sides..
	ProcessCollisionsWithScreenWalls();

	float sourceRadius = GetCollisionRadius();
	Vector3 sourceLocation = GetLocation();

	//now let's iterate through the world and see what we hit...
	//note: since there's a small number of objects in our game, this is fine.
	//but in a real game, brute-force checking collisions against every other object is not efficient.
	//it would be preferable to use a quad tree or some other structure to minimize the
	//number of collisions that need to be tested.
	for (auto goIt = World::sInstance->GetGameObjects().begin(), end = World::sInstance->GetGameObjects().end(); goIt != end; ++goIt)
	{
		GameObject* target = goIt->get();
		if (target != this && !target->DoesWantToDie())
		{
			//simple collision test for spheres- are the radii summed less than the distance?
			Vector3 targetLocation = target->GetLocation();
			float targetRadius = target->GetCollisionRadius();

			Vector3 delta = targetLocation - sourceLocation;
			float distSq = delta.LengthSq2D();
			float collisionDist = (sourceRadius + targetRadius);
			if (distSq < (collisionDist * collisionDist))
			{
				//first, tell the other guy there was a collision with a cat, so it can do something...

				if (target->HandleCollisionWithCat(this))
				{
					//okay, you hit something!
					//so, project your location far enough that you're not colliding
					Vector3 dirToTarget = delta;
					dirToTarget.Normalize2D();
					Vector3 acceptableDeltaFromSourceToTarget = dirToTarget * collisionDist;
					//important note- we only move this cat. the other cat can take care of moving itself
					SetLocation(targetLocation - acceptableDeltaFromSourceToTarget);


					Vector3 relVel = mVelocity;

					//if other object is a cat, it might have velocity, so there might be relative velocity...
					RoboCat* targetCat = target->GetAsCat();
					if (targetCat)
					{
						relVel -= targetCat->mVelocity;
					}

					//got vel with dir between objects to figure out if they're moving towards each other
					//and if so, the magnitude of the impulse ( since they're both just balls )
					float relVelDotDir = Dot2D(relVel, dirToTarget);

					if (relVelDotDir > 0.f)
					{
						Vector3 impulse = relVelDotDir * dirToTarget;

						if (targetCat)
						{
							mVelocity -= impulse;
							mVelocity *= mCatRestitution;
						}
						else
						{
							mVelocity -= impulse * 2.f;
							mVelocity *= mWallRestitution;
						}

					}
				}
			}
		}
	}

}

void RoboCat::ProcessCollisionsWithScreenWalls()
{
	Vector3 location = GetLocation();
	float x = location.mX;
	float y = location.mY;

	float vx = mVelocity.mX;
	float vy = mVelocity.mY;

	float radius = GetCollisionRadius();

	//if the cat collides against a wall, the quick solution is to push it off
	if ((y + radius) >= WORLD_HEIGHT && vy > 0)
	{
		mVelocity.mY = -vy * mWallRestitution;
		location.mY = WORLD_HEIGHT - radius;
		SetLocation(location);
	}
	else if (y - radius <= 0 && vy < 0)
	{
		mVelocity.mY = -vy * mWallRestitution;
		location.mY = radius;
		SetLocation(location);
	}

	if ((x + radius) >= WORLD_WIDTH && vx > 0)
	{
		mVelocity.mX = -vx * mWallRestitution;
		location.mX = WORLD_WIDTH - radius;
		SetLocation(location);
	}
	else if (x - radius <= 0 && vx < 0)
	{
		mVelocity.mX = -vx * mWallRestitution;
		location.mX = radius;
		SetLocation(location);
	}
}

uint32_t RoboCat::Write(OutputMemoryBitStream& inOutputStream, uint32_t inDirtyState) const
{
	uint32_t writtenState = 0;

	// ——— PlayerId ———
	bool dirty = (inDirtyState & ECRS_PlayerId) != 0;
	inOutputStream.Write(dirty);
	if (dirty)
	{
		inOutputStream.Write(GetPlayerId());
		writtenState |= ECRS_PlayerId;
	}

	// ——— Pose ———
	dirty = (inDirtyState & ECRS_Pose) != 0;
	inOutputStream.Write(dirty);
	if (dirty)
	{
		Vector3 vel = mVelocity;
		inOutputStream.Write(vel.mX);
		inOutputStream.Write(vel.mY);

		Vector3 loc = GetLocation();
		inOutputStream.Write(loc.mX);
		inOutputStream.Write(loc.mY);

		inOutputStream.Write(GetRotation());
		inOutputStream.Write(mInputDirection.mX);
		inOutputStream.Write(mInputDirection.mY);
		writtenState |= ECRS_Pose;
	}


	// ——— ThrustDir (always two bits) ———
	//if (mThrustDir != 0.f)
	//{
	//	inOutputStream.Write(true);
	//	inOutputStream.Write(mThrustDir > 0.f);
	//}
	//else
	//{
	//	inOutputStream.Write(false);
	//}




	// ——— Color ———
	dirty = (inDirtyState & ECRS_Color) != 0;
	inOutputStream.Write(dirty);
	if (dirty)
	{
		inOutputStream.Write(GetColor());
		writtenState |= ECRS_Color;
	}

	// ——— Health ———
	dirty = (inDirtyState & ECRS_Health) != 0;
	inOutputStream.Write(dirty);
	if (dirty)
	{
		inOutputStream.Write(mHealth, 5);
		writtenState |= ECRS_Health;
	}

	// ——— Machine-Gun Timer ———
	dirty = (inDirtyState & ECRS_MachineGunTimer) != 0;
	inOutputStream.Write(dirty);
	if (dirty)
	{
		float raw = mMachineGunTimer * 10.f;
		float clamped = std::max(0.f, std::min(raw, 255.f));
		uint8_t q = static_cast<uint8_t>(clamped);
		inOutputStream.Write(q, 8);
		writtenState |= ECRS_MachineGunTimer;
	}

	// ——— Invincibility Timer ———
	dirty = (inDirtyState & ECRS_InvincibilityTimer) != 0;
	inOutputStream.Write(dirty);
	if (dirty)
	{
		// pack 0–MaxInvTime seconds into 0–255 with 0.1s precision
		float raw = mInvincibilityTimer * 10.f;
		float clamped = std::max(0.f, std::min(raw, 255.f));
		uint8_t q = static_cast<uint8_t>(clamped);
		inOutputStream.Write(q, 8);
		//inOutputStream.Write(mInvincibilityTimer);
		writtenState |= ECRS_InvincibilityTimer;
	}

	return writtenState;
}



