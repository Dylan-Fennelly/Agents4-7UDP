#pragma once
class StateStack : sf::NonCopyable
{
public:
	enum class Action
	{
		Push,
		Pop,
		Clear
	};

	explicit StateStack();
	template <typename T>
	void RegisterState(StateID state_id);
	template <class T, class Param1>
	void RegisterState(StateID state_id, Param1 arg1);
	void Update(float dt);
	void HandleEvent(const sf::Event& event);

	void PushState(StateID state_id);
	void PopState();
	void ClearStates();

	bool IsEmpty() const;
	State* Current() const;
	void Render() const;

private:
	State::Ptr CreateState(StateID state_id);
	void ApplyPendingChanges();

	struct PendingChange
	{
		explicit PendingChange(Action action, StateID state_id = StateID::kNone);
		Action m_action;
		StateID m_state_id;
	};

	std::vector<State::Ptr> m_stack;
	std::vector<PendingChange> m_pending_list;
	std::map<StateID, std::function<State::Ptr()>> m_state_factory;
};

template <typename T>
void StateStack::RegisterState(const StateID state_id)
{
	m_state_factory[state_id] = [this]
		{
			return std::make_unique<T>(*this); // ⬅️ Pass the stack
		};
}

template <typename T, typename Param1>
void StateStack::RegisterState(const StateID state_id, Param1 arg1)
{
	m_state_factory[state_id] = [this, arg1]
		{
			return std::make_unique<T>(*this,arg1);
		};
}
