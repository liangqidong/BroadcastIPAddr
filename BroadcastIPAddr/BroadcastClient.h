#ifndef BROADCAST_CLIENT_H
#define BROADCAST_CLIENT_H
#include "UDPSocket.h"

class BroadcastClient
{
public:
	BroadcastClient();
	~BroadcastClient();

private:
	UDPSocket m_udpSocket;

};
#endif//BROADCAST_CLIENT_H


