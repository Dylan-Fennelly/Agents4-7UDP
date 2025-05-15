/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

//Based on RoboCat.cpp

class Zombie : public GameObject
{
public:
    CLASS_IDENTIFICATION('ZOMB', GameObject)

    enum EZombieType : uint8_t
    {
        ZT_Default = 0,
        ZT_Fast = 1,
        ZT_Count
    };

    enum EZombieReplicationState
    {
        ZRS_Pose = 1 << 0,
        ZRS_Health = 1 << 1,
        ZRS_Type = 1 << 2,
		ZRS_AllState = ZRS_Pose | ZRS_Health | ZRS_Type
    };

    static GameObject* StaticCreate() { return new Zombie(); }
    virtual uint32_t GetAllStateMask() const override { return ZRS_AllState; }

    void SetType(EZombieType t);
    int GetType() const { return mType; }

    void SetHealth(int h) { mHealth = h; }
    int  GetHealth() const { return mHealth; }
    void SetVelocity(const Vector3& v) { mVelocity = v; }
    const Vector3& GetVelocity() const { return mVelocity; }
    void SetMovementDirection(const Vector3& d) { mMovementDirection = d; }
    const Vector3& GetMovementDirection() const { return mMovementDirection; }
    void SimulateMovement(float inDeltaTime);


    virtual void Update() override;
    virtual uint32_t Write(OutputMemoryBitStream& out, uint32_t inDirtyState) const override;

protected:
    Zombie();

private:
    uint8_t      mType{ ZT_Default };
    Vector3 mVelocity;
    Vector3 mMovementDirection;
    int     mHealth{ 1 };
};