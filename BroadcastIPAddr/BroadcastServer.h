#ifndef BROADCAST_SERVER_H
#define BROADCAST_SERVER_H
#include "UDPSocket.h"

class BroadcastServer
{
public:
	BroadcastServer();
	~BroadcastServer();
	long SetServerPort(unsigned int port);
	long SendData(char* data, unsigned int& dataLen);

private:
	UDPSocket m_udpSocket;

};

#endif//BROADCAST_SERVER_H
