/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

class ScoreBoardManager
{
public:

	static void StaticInit();
	static std::unique_ptr< ScoreBoardManager >	sInstance;

	class Entry
	{
	public:
		Entry() {};

		Entry(uint32_t inPlayerID, const string& inPlayerName, const Vector3& inColor);

		const Vector3& GetColor()		const { return mColor; }
		uint32_t		GetPlayerId()	const { return mPlayerId; }
		const string& GetPlayerName()	const { return mPlayerName; }
		const string& GetFormattedNameScore()	const { return mFormattedNameScore; }
		int				GetScore()		const { return mScore; }
		int             GetDeaths()    const { return mDeaths; }

		void			SetScore(int inScore);
		void            IncDeath() { ++mDeaths; UpdateFormattedName(); }

		bool			Write(OutputMemoryBitStream& inOutputStream) const;
		bool			Read(InputMemoryBitStream& inInputStream);
		static uint32_t	GetSerializedSize();

	private:
		Vector3			mColor;

		uint32_t		mPlayerId;
		string			mPlayerName;

		int				mScore;
		int             mDeaths = 0;

		string			mFormattedNameScore;

		void            UpdateFormattedName()
		{
			//A nicer way of displaying the scoreboard, as deaths are being counted too
			char buffer[256];
			snprintf(buffer, sizeof(buffer), "%s  K:%d  D:%d", mPlayerName.c_str(), mScore, mDeaths);
			mFormattedNameScore = buffer;
		}
	};

	Entry* GetEntry(uint32_t inPlayerId);
	bool	RemoveEntry(uint32_t inPlayerId);
	void	AddEntry(uint32_t inPlayerId, const string& inPlayerName);
	void	IncScore(uint32_t inPlayerId, int inAmount);
	void    SetDeath(uint32_t inPlayerId);

	bool	Write(OutputMemoryBitStream& inOutputStream) const;
	bool	Read(InputMemoryBitStream& inInputStream);

	const vector< Entry >& GetEntries()	const { return mEntries; }

	bool WriteToFile(const std::string& inFileName) const;

private:

	ScoreBoardManager();

	vector< Entry >	mEntries;

	vector< Vector3 >	mDefaultColors;
};

