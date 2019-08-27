#ifndef BROADCAST_SERVER_H
#define BROADCAST_SERVER_H
#include "BroadcastComDefine.h"

class BroadcastIPAddrServer
{
public:
	BroadcastIPAddrServer();
	~BroadcastIPAddrServer();
	long RegRecFun(BroadcastRecDataFun recDataFun);
	long BindBroadcastReceivePort(unsigned int port);
	long SendData(const char* data, unsigned int& dataLen, const char* addrIP = "0.0.0.0");
	long Process();
private:
	void* obj;
};

#ifdef __cplusplus
extern "C" {
#endif
	EXPORTDLL_API void CreateServerObj(BroadcastIPAddrServer* obj);
	EXPORTDLL_API void DestroyServerObj(BroadcastIPAddrServer* obj);
#ifdef __cplusplus
}
#endif
#endif//BROADCAST_SERVER_H
