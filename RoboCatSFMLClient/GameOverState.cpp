#include "RoboCatClientPCH.hpp"

#ifdef _WIN32
#include <direct.h>
#define MKDIR(path) _mkdir(path)
#else
#include <sys/stat.h>
#define MKDIR(path) mkdir(path, 0755)
#endif

GameOverState::GameOverState(StateStack& stack)
    : State(stack), m_time_remaining(15.f)
{
    m_background_sprite.setTexture(*TextureManager::sInstance->GetTexture("title"));

    // Sort scoreboard by kills (score)
    m_sorted_entries = ScoreBoardManager::sInstance->GetEntries();
    std::sort(m_sorted_entries.begin(), m_sorted_entries.end(),
        [](const auto& a, const auto& b) {
            return a.GetScore() > b.GetScore();
        });

    // Timer text styling
    m_timer_text.setFont(*FontManager::sInstance->GetFont("carlito"));
    m_timer_text.setCharacterSize(40);
    m_timer_text.setFillColor(sf::Color::Red);
    m_timer_text.setPosition(0.f, 30.f); // X will be centered in Draw
    UpdateTimerText();

    // Center X position for UI
    float centerX = WindowManager::sInstance->getView().getSize().x / 2.f;

    // "Back to Menu Now" button
    auto back_button = std::make_shared<gui::Button>();
    back_button->SetText("Back to Menu Now");
    back_button->setPosition(centerX - 100.f, 500); // Approximate centering
    back_button->SetCallback([this]() {
        RequestStackClear();
        RequestStackPush(StateID::kMenu);
        });

    // "Save to File" button
    auto save_button = std::make_shared<gui::Button>();
    save_button->SetText("Save Scoreboard");
    save_button->setPosition(centerX - 100.f, 560);
    save_button->SetCallback([this]() {
        static bool hasWrittenFile = false;
        if (hasWrittenFile)
            return;
        hasWrittenFile = true;
		m_ScoreBoardPrinted = true;
        const char* dir = "Scores";
        if (MKDIR(dir) != 0 && errno != EEXIST)
        {
            std::cerr << "[Server] ERROR: failed to create directory '"
                << dir << "' (errno=" << errno << ")\n";
        }

        auto now = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(now);
        std::tm tm;
#ifdef _WIN32
        localtime_s(&tm, &t);
#else
        localtime_r(&t, &tm);
#endif

        std::ostringstream oss;
        oss << dir << "/scoreboard_"
            << (tm.tm_year + 1900)
            << std::setw(2) << std::setfill('0') << (tm.tm_mon + 1)
            << std::setw(2) << std::setfill('0') << tm.tm_mday
            << '_'
            << std::setw(2) << std::setfill('0') << tm.tm_hour
            << std::setw(2) << std::setfill('0') << tm.tm_min
            << std::setw(2) << std::setfill('0') << tm.tm_sec
            << ".csv";

        const std::string fileName = oss.str();

        if (ScoreBoardManager::sInstance->WriteToFile(fileName))
        {
            std::cout << "[Server] Wrote final scoreboard to "
                << fileName << std::endl;
        }
        else
        {
            std::cerr << "[Server] ERROR: failed to write "
                << fileName << std::endl;
        }
        });

    // Add buttons to container
    m_container.Pack(back_button);
    m_container.Pack(save_button);

    AudioManager::sInstance->Play("gameWon");
}

void GameOverState::UpdateTimerText()
{
    int seconds = static_cast<int>(std::ceil(m_time_remaining));
    m_timer_text.setString("Returning in " + std::to_string(seconds) + "s");
}

void GameOverState::Draw()
{
    m_background_sprite.setColor(sf::Color(100, 100, 100, 200)); // Light grey with some transparency
    WindowManager::sInstance->draw(m_background_sprite);

    const float viewWidth = WindowManager::sInstance->getView().getSize().x;
    float yOffset = 120.f;

    // Draw centered scoreboard entries
    for (const auto& entry : m_sorted_entries)
    {
        sf::Text displayText;
        displayText.setFont(*FontManager::sInstance->GetFont("carlito"));
        displayText.setString(entry.GetFormattedNameScore());
        displayText.setCharacterSize(40);
        displayText.setFillColor(sf::Color(entry.GetColor().mX, entry.GetColor().mY, entry.GetColor().mZ));

        sf::FloatRect bounds = displayText.getLocalBounds();
        displayText.setOrigin(bounds.width / 2.f, 0.f);
        displayText.setPosition(viewWidth / 2.f, yOffset);

        WindowManager::sInstance->draw(displayText);
        yOffset += 50.f;
    }

    // Draw centered countdown timer
    sf::FloatRect timerBounds = m_timer_text.getLocalBounds();
    m_timer_text.setOrigin(timerBounds.width / 2.f, 0.f);
    m_timer_text.setPosition(viewWidth / 2.f, 30.f);
    WindowManager::sInstance->draw(m_timer_text);

    // Draw button container
    WindowManager::sInstance->draw(m_container);
}

bool GameOverState::Update(float dt)
{
    m_time_remaining -= dt;
    if (m_time_remaining <= 0.f)
    {
        RequestStackClear();
        RequestStackPush(StateID::kMenu);
    }
    else
    {
        UpdateTimerText();
    }

    return true;
}

bool GameOverState::HandleEvent(const sf::Event& event)
{
    if (m_time_remaining <= 12.f)
    {
        m_container.HandleEvent(event);
    }

    return false;
}
