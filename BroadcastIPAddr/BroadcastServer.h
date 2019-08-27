#ifndef BROADCAST_SERVER_H
#define BROADCAST_SERVER_H

#include "Broadcast.h"

class BroadcastServer
	: public BroadcastSocket
{
public:
	BroadcastServer();
	~BroadcastServer();
	long Process();
};

#endif//BROADCAST_SERVER_H
