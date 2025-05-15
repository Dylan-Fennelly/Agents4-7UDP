#pragma once

class GameOverState : public State
{
public:
    explicit GameOverState(StateStack& stack);

    void Draw() override;
    bool Update(float dt) override;
    bool HandleEvent(const sf::Event& event) override;

private:
    sf::Sprite m_background_sprite;
    gui::Container m_container;
    std::vector<ScoreBoardManager::Entry> m_sorted_entries;

    float m_time_remaining;
    sf::Text m_timer_text;

    void UpdateTimerText();

	bool m_ScoreBoardPrinted = false;
};
