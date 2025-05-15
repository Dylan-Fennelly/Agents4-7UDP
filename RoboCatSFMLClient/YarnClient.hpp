class YarnClient : public Yarn
{
public:
	static	GameObjectPtr	StaticCreate() 
	{ 
		AudioManager::sInstance->Play("shot");
		return GameObjectPtr(new YarnClient()); 
	}

	virtual void		Read(InputMemoryBitStream& inInputStream) override;

protected:
	YarnClient();

private:

	SpriteComponentPtr	mSpriteComponent;
};
