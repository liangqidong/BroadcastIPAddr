#ifndef BROADCAST_SOCKET_H
#define BROADCAST_SOCKET_H
#include <functional>

#ifndef BroadcastRecDataFun
#define BroadcastRecDataFun std::function<long(const char* ip,unsigned int& port, const char* data, unsigned int dataLen)> 
#endif//BroadcastRecDataFun

class BroadcastSocket
{
public:
	BroadcastSocket();
	virtual ~BroadcastSocket();

};
#endif//BROADCAST_SOCKET_H
