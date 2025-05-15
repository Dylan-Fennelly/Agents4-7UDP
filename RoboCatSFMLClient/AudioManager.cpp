#include "RoboCatClientPCH.hpp"

namespace
{
	// Sound coordinate system, point of view of a player in front of the screen:
	// X = left; Y = up; Z = back (out of the screen)
	constexpr float ListenerZ = 300.f;
	constexpr float Attenuation = 8.f;
	constexpr float MinDistance2D = 200.f;
	const float MinDistance3D = std::sqrt(MinDistance2D * MinDistance2D + ListenerZ * ListenerZ);
}

std::unique_ptr<AudioManager> AudioManager::sInstance;

void AudioManager::StaticInit()
{
	sInstance.reset(new AudioManager());
}

AudioManager::AudioManager()
	: m_music_volume(30.f)
{
	CacheSound("shot", "../Assets/Media/Sounds/PistolShot.wav");
	CacheSound("button", "../Assets/Media/Sounds/Button.wav");
	CacheSound("zombie", "../Assets/Media/Sounds/DyingZombie.wav");
	CacheSound("pickup", "../Assets/Media/Sounds/Pickup.wav");	
	CacheSound("death", "../Assets/Media/Sounds/Dying.wav");
	CacheSound("gameWon", "../Assets/Media/Music/GameWonTheme.wav");

	CacheMusic("menu", "../Assets/Media/Music/MenuTheme.wav");
	
	CacheMusic("gameOver", "../Assets/Media/Music/GameOverTheme.wav");
	CacheMusic("gameplay", "../Assets/Media/Music/GameplayTheme.wav");
	
	// Listener points towards the screen (default in SFML)
	sf::Listener::setDirection(0.f, 0.f, -1.f);
}

SoundPtr AudioManager::GetSound(const string& inSoundName)
{
	return mNameToSoundMap.at(inSoundName);
}

MusicPtr AudioManager::GetMusic(const string& inMusicName)
{
	return mNameToMusicMap.at(inMusicName);
}

std::string AudioManager::GetCurrentMusicName() const
{
	return m_current_music_name;
}

void AudioManager::Play(const string& sound_name)
{
	Play(sound_name, GetListenerPosition());
}

void AudioManager::Play(const string& sound_name, sf::Vector2f position)
{
	m_sounds.emplace_back(sf::Sound());
	sf::Sound& sound = m_sounds.back();

	sound.setBuffer(*GetSound(sound_name));
	sound.setPosition(position.x, -position.y, 0.f);
	sound.setAttenuation(Attenuation);
	sound.setMinDistance(MinDistance3D);

	sound.play();
}

void AudioManager::RemoveStoppedSounds()
{
	m_sounds.remove_if([](const sf::Sound& s)
		{
			return s.getStatus() == sf::Sound::Stopped;
		});
}

void AudioManager::SetListenerPosition(sf::Vector2f position) const
{
	sf::Listener::setPosition(position.x, -position.y, ListenerZ);
}

sf::Vector2f AudioManager::GetListenerPosition() const
{
	sf::Vector3f position = sf::Listener::getPosition();
	return { position.x, -position.y };
}

void AudioManager::PlayMusic(const string& music_name, bool force_restart)
{
	if (!force_restart && m_current_music_name == music_name)
		return;

	StopMusic();

	m_music = mNameToMusicMap.at(music_name).get();
	m_music->setVolume(m_music_volume);
	m_music->setLoop(true);
	m_music->play();
	m_current_music_name = music_name;
}

void AudioManager::StopMusic()
{
	if (m_music != nullptr)
		m_music->stop();

	m_current_music_name = "";
}

void AudioManager::PauseMusic(bool paused) const
{
	if (m_music == nullptr)
		return;

	if (paused)
		m_music->pause();
	else
		m_music->play();
}

void AudioManager::SetMusicVolume(float volume)
{
	m_music_volume = volume;

	if (m_music != nullptr)
		m_music->setVolume(volume);
}

bool AudioManager::CacheSound(const string& inName, const char* inFileName)
{
	const SoundPtr newSound(new sf::SoundBuffer());
	if (!newSound->loadFromFile(inFileName))
	{
		return false;
	}

	mNameToSoundMap[inName] = newSound;

	return true;
}

bool AudioManager::CacheMusic(const string& inName, const char* inFileName)
{
	const MusicPtr newMusic(new sf::Music());
	if (!newMusic->openFromFile(inFileName))
	{
		return false;
	}

	mNameToMusicMap[inName] = newMusic;

	return true;
}
