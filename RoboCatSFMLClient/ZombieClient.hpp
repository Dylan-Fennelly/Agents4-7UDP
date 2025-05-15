/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

class ZombieClient : public Zombie
{
public:
    static GameObjectPtr StaticCreate() { return GameObjectPtr(new ZombieClient()); }

    virtual void Update() override;
    virtual void HandleDying() override;
    virtual void Read(InputMemoryBitStream& inInputStream) override;

    void DoClientSidePredictionAfterReplicationForRemoteZombie(uint32_t inReadState);

protected:
    ZombieClient();

private:
    void InterpolateClientSidePrediction(
        float inOldRotation,
        const Vector3& inOldLocation,
        const Vector3& inOldVelocity);

    float               mTimeLocationBecameOutOfSync;
    float               mTimeVelocityBecameOutOfSync;

    SpriteComponentPtr  mSpriteComponent;
};