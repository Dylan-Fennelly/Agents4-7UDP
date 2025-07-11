/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

#include "RoboCatServerPCH.hpp"

namespace
{
	const float kRespawnDelay = 3.f;
}

ClientProxy::ClientProxy(const SocketAddress& inSocketAddress, const string& inName, int inPlayerId) : 
	mSocketAddress(inSocketAddress),
	mName(inName),
	mPlayerId(inPlayerId),
	mDeliveryNotificationManager(false, true),
	mIsLastMoveTimestampDirty(false),
	mTimeToRespawn(0.f)
{
	UpdateLastPacketTime();
}


void ClientProxy::UpdateLastPacketTime()
{
	mLastPacketFromClientTime = Timing::sInstance.GetTimef();
}

void	ClientProxy::HandleCatDied()
{
	mTimeToRespawn = Timing::sInstance.GetFrameStartTime() + kRespawnDelay;
}

void	ClientProxy::RespawnCatIfNecessary()
{
	if (mTimeToRespawn != 0.f && Timing::sInstance.GetFrameStartTime() > mTimeToRespawn)
	{
		static_cast<Server*> (Engine::s_instance.get())->SpawnCatForPlayer(mPlayerId);
		mTimeToRespawn = 0.f;
	}
}


