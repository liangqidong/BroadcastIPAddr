#include "BroadcastServer.h"

BroadcastServer::BroadcastServer()
{
}


BroadcastServer::~BroadcastServer()
{
	
}

long BroadcastServer::Process()
{
// 	UDPSocket** udpSocket = nullptr;
// 	std::thread** tThreadobj = nullptr;
// 	getUDPSocket(udpSocket);
// 	(*udpSocket)->bindSocket();
// 	getThreadobj(tThreadobj);
// 	if (nullptr != *tThreadobj)
// 	{
// 		return 0;
// 	}
// 	*tThreadobj = new std::thread(&BroadcastServer::RecDataThread,this);

	m_udpSocket.bindSocket();
	m_recDataThread = new std::thread(&BroadcastServer::RecDataThread, this);
	return 0;
}
