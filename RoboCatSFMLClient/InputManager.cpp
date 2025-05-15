/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

#include "RoboCatClientPCH.hpp"

unique_ptr< InputManager >	InputManager::sInstance;

namespace
{
	float kTimeBetweenInputSamples = 0.03f;
}

void InputManager::StaticInit()
{
	sInstance.reset(new InputManager());
}


namespace
{
	inline void UpdateDesireVariableFromKey(EInputAction inInputAction, bool& ioVariable)
	{
		if (inInputAction == EIA_Pressed)
		{
			ioVariable = true;
		}
		else if (inInputAction == EIA_Released)
		{
			ioVariable = false;
		}
	}

	inline void UpdateDesireFloatFromKey(EInputAction inInputAction, float& ioVariable)
	{
		if (inInputAction == EIA_Pressed)
		{
			ioVariable = 1.f;
		}
		else if (inInputAction == EIA_Released)
		{
			ioVariable = 0.f;
		}
	}
}

void InputManager::HandleInput(EInputAction inInputAction, int inKeyCode)
{
	Vector3 pos;
	switch (inKeyCode)
	{
	case sf::Keyboard::A:
		UpdateDesireFloatFromKey(inInputAction, mCurrentState.mDesiredLeftAmount);
		break;
	case sf::Keyboard::D:
		UpdateDesireFloatFromKey(inInputAction, mCurrentState.mDesiredRightAmount);
		break;
	case sf::Keyboard::W:
		UpdateDesireFloatFromKey(inInputAction, mCurrentState.mDesiredForwardAmount);
		break;
	case sf::Keyboard::S:
		UpdateDesireFloatFromKey(inInputAction, mCurrentState.mDesiredBackAmount);
		break;
	case sf::Keyboard::Space:
		UpdateDesireVariableFromKey(inInputAction, mCurrentState.mIsShooting);
		break;
	case sf::Keyboard::Escape:
	{
		if (inInputAction == EIA_Pressed)
		{
			Pause();
			StackManager::sInstance->PushState(StateID::kPause);
		}
		break;
	}
	/*case sf::Keyboard::Add:
	case sf::Keyboard::Equal:
	{
		float latency = NetworkManagerClient::sInstance->GetSimulatedLatency();
		latency += 0.1f;
		if (latency > 0.5f)
		{
			latency = 0.5f;
		}
		NetworkManagerClient::sInstance->SetSimulatedLatency(latency);
		break;
	}*/
	/*case sf::Keyboard::Subtract:
	{
		float latency = NetworkManagerClient::sInstance->GetSimulatedLatency();
		latency -= 0.1f;
		if (latency < 0.0f)
		{
			latency = 0.0f;
		}
		NetworkManagerClient::sInstance->SetSimulatedLatency(latency);
		break;
	}*/
	}

}


InputManager::InputManager() :
	mNextTimeToSampleInput(0.f),
	mPendingMove(nullptr)
{

}

const Move& InputManager::SampleInputAsMove()
{
	return mMoveList.AddMove(GetState(), Timing::sInstance.GetFrameStartTime());
}

bool InputManager::IsTimeToSampleInput()
{
	float time = Timing::sInstance.GetFrameStartTime();
	if (time > mNextTimeToSampleInput)
	{
		mNextTimeToSampleInput = mNextTimeToSampleInput + kTimeBetweenInputSamples;
		return true;
	}

	return false;
}

void InputManager::Update()
{
	if (IsTimeToSampleInput())
	{
		mPendingMove = &SampleInputAsMove();
		CalculateRotation();

	}
}

void InputManager::CalculateRotation()
{
	Vector3 pos = NetworkManagerClient::sInstance->GetLocalCatPosition();
	sf::Vector2f posF(pos.mX, pos.mY);

	sf::Vector2i mousePos = sf::Mouse::getPosition(*WindowManager::sInstance);
	sf::Vector2f mousePosF = WindowManager::sInstance->mapPixelToCoords(mousePos, RenderManager::sInstance->GetView());

	sf::Vector2f dir = mousePosF - posF;
	float angle = atan2f(dir.y, dir.x) * (180.f / 3.14159265f);
	angle += 90.f;
	angle = fmodf(angle + 360.f, 360.f);

	// Quantise to the same step used for network
	float quantisedAngle = std::round((angle / 360.f) * 255.f) / 255.f * 360.f;
	mCurrentState.mDesiredRotation = quantisedAngle;
	//std::cout << "angle: " << angle << " quantisedAngle: " << quantisedAngle << std::endl;
}

