#ifndef BROADCAST_IP_ADDR_SERVER_H
#define BROADCAST_IP_ADDR_SERVER_H

#include "Broadcast.h"

class BroadcastServer
	: public BroadcastSocket
{
public:
	BroadcastServer();
	~BroadcastServer();
	long Process();
};

#endif//BROADCAST_IP_ADDR_SERVER_H
