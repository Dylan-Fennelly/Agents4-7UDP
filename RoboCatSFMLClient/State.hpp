/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

#pragma once
class StateStack;

class State
{
public:
	using Ptr = std::unique_ptr<State>;

	explicit State(StateStack& stack) : m_stack(stack) {}
	virtual ~State() = default;

	virtual bool Update(float dt) = 0;
	virtual bool HandleEvent(const sf::Event& event) = 0;
	virtual void OnStackPopped();
	virtual void OnStackPushed();
	static void RequestStackPush(StateID state_id);
	virtual void Draw() = 0;

protected:
	static void RequestStackPop();
	static void RequestStackClear();

protected:
	StateStack& m_stack;
};
