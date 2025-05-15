#include "RoboCatClientPCH.hpp"


void State::OnStackPopped()
{
	//Does nothing. Can be overridden in child classes to do something if the state is popped.
	//(This is needed because certain operations can't be done in the destructor)
}

void State::OnStackPushed()
{
	//Every state sets the camera to the default position
	WindowManager::sInstance->setView(WindowManager::sInstance->getView());
}

void State::RequestStackPush(const StateID state_id)
{
	StackManager::sInstance->PushState(state_id);
}

void State::RequestStackPop()
{
	StackManager::sInstance->PopState();
}

void State::RequestStackClear()
{
	StackManager::sInstance->ClearStates();
}
