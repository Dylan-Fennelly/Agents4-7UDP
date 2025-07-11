/*Albert Skalinski - D00248346
  Dylan Fennelly - D00248176*/

class UDPSocket
{
public:

	~UDPSocket();

	int Bind(const SocketAddress& inToAddress);
	int SendTo(const void* inToSend, int inLength, const SocketAddress& inToAddress);
	int ReceiveFrom(void* inToReceive, int inMaxLength, SocketAddress& outFromAddress);

	int SetNonBlockingMode(bool inShouldBeNonBlocking);

private:
	friend class SocketUtil;
	UDPSocket(SOCKET inSocket) : mSocket(inSocket) {}
	SOCKET mSocket;

};

typedef shared_ptr< UDPSocket >	UDPSocketPtr;

