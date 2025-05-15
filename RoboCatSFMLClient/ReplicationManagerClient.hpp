/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

class ReplicationManagerClient
{
public:
	void Read(InputMemoryBitStream& inInputStream);

private:

	void ReadAndDoCreateAction(InputMemoryBitStream& inInputStream, int inNetworkId);
	void ReadAndDoUpdateAction(InputMemoryBitStream& inInputStream, int inNetworkId);
	void ReadAndDoDestroyAction(InputMemoryBitStream& inInputStream, int inNetworkId);

};

