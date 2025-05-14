#pragma once
class StateStack;

class State
{
public:
	using Ptr = std::unique_ptr<State>;

	explicit State() = default;
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
};
