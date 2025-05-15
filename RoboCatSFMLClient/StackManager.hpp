/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

#pragma once
//implementation borrowed from Colourjump (which was adapted from the SFML Game Development Book)
class StackManager
{
public:
	static bool StaticInit();
	void PushState(StateID title);
	void Update(float delta_time);
	bool IsEmpty() const;
	void HandleEvent(const sf::Event& p_event);
	void Render() const;
	void PopState();
	void ClearStates();
	static std::unique_ptr<StackManager> sInstance;
	State* GetCurrentState() const;

private:
	StackManager();

	StateStack m_stack;
};

