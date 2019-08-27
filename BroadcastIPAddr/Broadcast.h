#ifndef BROADCAST_SOCKET_H
#define BROADCAST_SOCKET_H
#include "UDPSocket.h"
#include <thread>

#include "BroadcastComDefine.h"

class BroadcastSocket
{
public:
	BroadcastSocket();
	virtual ~BroadcastSocket();
	virtual long SendData(const char* data, unsigned int& dataLen, const char* addrIP = "0.0.0.0");//if addrIP is "0.0.0.0"  addIP isBroadcast
	virtual long RegRecFun(BroadcastRecDataFun recDataFun);
	virtual long BindReceivePort(unsigned int port);
	virtual long bindSendPort(unsigned int port);
	virtual long Process();

protected:
	virtual void RecDataThread();
	long getUDPSocket(UDPSocket** udpSocket);
	long getThreadobj(std::thread** tThreadobj);
	std::thread*			m_recDataThread;
	UDPSocket				m_udpSocket;

private:
	BroadcastRecDataFun		m_recDataFun;
	bool					m_threadQuit;
};
#endif//BROADCAST_SOCKET_H
