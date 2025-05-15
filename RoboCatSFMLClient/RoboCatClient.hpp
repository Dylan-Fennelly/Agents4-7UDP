/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

class RoboCatClient : public RoboCat
{
public:
	static	GameObjectPtr	StaticCreate() { return GameObjectPtr(new RoboCatClient()); }

	virtual void Update();
	virtual void	HandleDying() override;

	virtual void	Read(InputMemoryBitStream& inInputStream) override;

	void DoClientSidePredictionAfterReplicationForLocalCat(uint32_t inReadState);
	void DoClientSidePredictionAfterReplicationForRemoteCat(uint32_t inReadState);

	const std::string& GetDefaultTextureName() const { return mDefaultTextureName; }

	SpriteComponent* GetSpriteComponent() const { return mSpriteComponent.get(); }

protected:
	RoboCatClient();
private:
	void InterpolateClientSidePrediction(float inOldRotation, const Vector3& inOldLocation, const Vector3& inOldVelocity, bool inIsForRemoteCat);
	float				mTimeLocationBecameOutOfSync;
	float				mTimeVelocityBecameOutOfSync;

	SpriteComponentPtr	mSpriteComponent;
	std::string           mDefaultTextureName;
};

