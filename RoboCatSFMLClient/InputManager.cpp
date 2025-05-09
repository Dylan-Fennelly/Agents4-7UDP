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
	case sf::Keyboard::K:
		UpdateDesireVariableFromKey(inInputAction, mCurrentState.mIsShooting);
		break;
	case sf::Keyboard::Add:
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
	}
	case sf::Keyboard::Subtract:
	{
		float latency = NetworkManagerClient::sInstance->GetSimulatedLatency();
		latency -= 0.1f;
		if (latency < 0.0f)
		{
			latency = 0.0f;
		}
		NetworkManagerClient::sInstance->SetSimulatedLatency(latency);
		break;
	}
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
	std::cout << "InputManager::Update() pos: " << pos.mX << ", " << pos.mY << ", " << pos.mZ << std::endl;
	sf::Vector2i mousePos = sf::Mouse::getPosition(*WindowManager::sInstance);
	sf::Vector2f mousePosF = WindowManager::sInstance->mapPixelToCoords(mousePos, RenderManager::sInstance->GetView());
	//convert pos into sf::Vector2f
	sf::Vector2f posF(pos.mX, pos.mY);
	sf::Vector2f dir = mousePosF - posF;
	float angle = atan2(dir.y, dir.x) * 180.f / 3.14159265f;
	angle = angle + 90.f;
	mCurrentState.mDesiredRotation = angle;
}
