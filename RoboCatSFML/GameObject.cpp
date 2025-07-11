/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

#include "RoboCatPCH.hpp"

GameObject::GameObject() :
	mIndexInWorld(-1),
	mCollisionRadius(1.f),
	mDoesWantToDie(false),
	mRotation(0.f),
	mNetworkId(0),
	mColor(Colors::White),
	mScale(1.0f)
{
}

void GameObject::Update()
{
	//object don't do anything by default...	
}


Vector3 GameObject::GetForwardVector()	const
{
	//should we cache this when you turn?
	//SDL appears to use radians while SFML uses degrees - make sure to convert to radians
	//before using!
	return Vector3(sinf(RoboMath::ToRadians(mRotation)), -cosf(RoboMath::ToRadians(mRotation)), 0.f);
}

void GameObject::SetNetworkId(int inNetworkId)
{
	//this doesn't put you in the map or remove you from it
	mNetworkId = inNetworkId;

}

void GameObject::SetRotation(float inRotation)
{
	//should we normalize using fmodf?
	mRotation = inRotation;
}
