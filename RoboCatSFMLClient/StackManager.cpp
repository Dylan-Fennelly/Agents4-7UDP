/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

#include "RoboCatClientPCH.hpp"
std::unique_ptr<StackManager> StackManager::sInstance;

bool StackManager::StaticInit()
{
	sInstance.reset(new StackManager());
	return true;
}

void StackManager::PushState(const StateID title)
{
	m_stack.PushState(title);
}

void StackManager::Update(const float delta_time)
{
	m_stack.Update(delta_time);
}

bool StackManager::IsEmpty() const
{
	return m_stack.IsEmpty();
}

void StackManager::HandleEvent(const sf::Event& p_event)
{
	m_stack.HandleEvent(p_event);
}

void StackManager::Render() const
{
	m_stack.Render();
}

void StackManager::PopState()
{
	m_stack.PopState();
}

void StackManager::ClearStates()
{
	m_stack.ClearStates();
}

State* StackManager::GetCurrentState() const
{
	return m_stack.Current();
}
StackManager::StackManager()
{
	//lets register all the states
	m_stack.RegisterState<TitleState>(StateID::kTitleScreen);
	m_stack.RegisterState<GameState>(StateID::kGameHost,true);
	m_stack.RegisterState<GameState>(StateID::kGameClient, false);
	m_stack.RegisterState<CredentialEntryState>(StateID::kCredentialEntryJoin,false);
	m_stack.RegisterState<CredentialEntryState>(StateID::kCredentialEntryHost, true);
	m_stack.RegisterState<MenuState>(StateID::kMenu);
	m_stack.RegisterState<PauseState>(StateID::kPause);
	m_stack.RegisterState<GameOverState>(StateID::kGameOver);
	m_stack.RegisterState<InstructionState>(StateID::kInstructionsJoin,false);
	m_stack.RegisterState<InstructionState>(StateID::kInstructionsHost, true);
	m_stack.PushState(StateID::kTitleScreen);
}
