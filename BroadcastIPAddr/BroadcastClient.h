#ifndef BROADCAST_CLIENT_H
#define BROADCAST_CLIENT_H
#include "UDPSocket.h"
#include "Broadcast.h"
#include <thread>

class BroadcastClient
{
public:
	BroadcastClient();
	~BroadcastClient();
	long SendData(const char* data, unsigned int& dataLen, const char* addrIP = "0.0.0.0");//if addrIP is "0.0.0.0"  addIP isBroadcast
	long RegRecFun(BroadcastRecDataFun recDataFun);
	long BindReceivePort(unsigned int port);
	long bindSendPort(unsigned int port);
	long Process();
private:
	void RecDataThread();

	UDPSocket				m_udpSocket;
	BroadcastRecDataFun		m_recDataFun;
	std::thread*			m_recDataThread;

};
#endif//BROADCAST_CLIENT_H


