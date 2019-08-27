#include "BroadcastServer.h"
#include "BroadcastErrorCode.h"

BroadcastServer::BroadcastServer()
{
}


BroadcastServer::~BroadcastServer()
{
	
}

long BroadcastServer::Process()
{
	m_udpSocket.bindSocket();
	m_recDataThread = new std::thread(&BroadcastServer::RecDataThread, this);
	return BEC_SUCCESS;
}
