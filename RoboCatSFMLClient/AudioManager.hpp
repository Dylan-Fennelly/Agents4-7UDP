/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

//Ported from ColorJump

typedef shared_ptr<sf::SoundBuffer> SoundPtr;
typedef shared_ptr<sf::Music> MusicPtr;

class AudioManager
{
public:
	static void StaticInit();
	static std::unique_ptr<AudioManager> sInstance;

	SoundPtr GetSound(const string& inSoundName);
	MusicPtr GetMusic(const string& inMusicName);
	string GetCurrentMusicName() const;
	void Play(const string& sound_name);
	void Play(const string& sound_name, sf::Vector2f position);
	void RemoveStoppedSounds();
	void SetListenerPosition(sf::Vector2f position) const;
	sf::Vector2f GetListenerPosition() const;

	void PlayMusic(const string& music_name, bool force_restart = false);
	void StopMusic();
	void PauseMusic(bool paused) const;
	void SetMusicVolume(float volume);

private:
	AudioManager();

	bool CacheSound(const string& inName, const char* inFileName);
	bool CacheMusic(const string& inName, const char* inFileName);
	unordered_map<string, SoundPtr> mNameToSoundMap;
	unordered_map<string, MusicPtr> mNameToMusicMap;
	std::list<sf::Sound> m_sounds;

	std::string m_current_music_name;
	sf::Music* m_music{};
	float m_music_volume;
};



