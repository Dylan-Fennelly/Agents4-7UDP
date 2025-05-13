class Zombie : public GameObject
{
public:
    CLASS_IDENTIFICATION('ZOMB', GameObject)

        enum EZombieReplicationState
    {
        ZRS_Pose = 1 << 0,  // pos, rot, vel, movement dir
        ZRS_Health = 1 << 1,
        ZRS_AllState = ZRS_Pose | ZRS_Health
    };

    static GameObject* StaticCreate() { return new Zombie(); }
    virtual uint32_t GetAllStateMask() const override { return ZRS_AllState; }

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
    Vector3 mVelocity;
    Vector3 mMovementDirection;
    int     mHealth{ 100 };
};