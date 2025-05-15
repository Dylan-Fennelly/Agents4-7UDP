/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

class Timing
{
public:

	Timing();

	void Update();

	float GetDeltaTime() const { return mDeltaTime; }

	double GetTime() const;

	float GetTimef() const
	{
		return static_cast<float>(GetTime());
	}

	float GetFrameStartTime() const { return mFrameStartTimef; }


	static Timing sInstance;

private:
	float		mDeltaTime;
	uint64_t	mDeltaTick;

	double		mLastFrameStartTime;
	float		mFrameStartTimef;
	double		mPerfCountDuration;
};

