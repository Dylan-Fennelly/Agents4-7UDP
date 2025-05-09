class InputState
{
public:

	InputState() :
		mDesiredRightAmount(0),
		mDesiredLeftAmount(0),
		mDesiredForwardAmount(0),
		mDesiredBackAmount(0),
		mIsShooting(false),
		mDesiredRotation(0.f)
	{}

	float GetDesiredHorizontalDelta()	const { return /*1.f;*/mDesiredRightAmount - mDesiredLeftAmount; }
	float GetDesiredVerticalDelta()		const { return mDesiredForwardAmount - mDesiredBackAmount; }
	bool  IsShooting()					const { return mIsShooting; }

	float GetDesiredRotation() const { return mDesiredRotation; }

	bool Write(OutputMemoryBitStream& inOutputStream) const;
	bool Read(InputMemoryBitStream& inInputStream);

private:
	friend class InputManager;

	float mDesiredRightAmount, mDesiredLeftAmount;
	float mDesiredForwardAmount, mDesiredBackAmount;
	bool mIsShooting;
	float mDesiredRotation;
};
