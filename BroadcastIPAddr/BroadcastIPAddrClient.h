#ifndef BROADCAST_IP_ADDR_CLENT_H
#define BROADCAST_IP_ADDR_CLENT_H
#include "BroadcastComDefine.h"

class BroadcastIPAddrClient
{
public:
	BroadcastIPAddrClient();
	~BroadcastIPAddrClient();
	long RegRecFun(BroadcastRecDataFun recDataFun);
	long BindBroadcastServerPort(unsigned int port);
	long SendData(const char* data, unsigned int& dataLen, const char* addrIP = "0.0.0.0");
	long Process();
private:
	void* obj;
};

#ifdef __cplusplus
extern "C" {
#endif
	EXPORTDLL_API void CreateClientObj(BroadcastIPAddrClient* obj);
	EXPORTDLL_API void DestroyClientObj(BroadcastIPAddrClient* obj);
#ifdef __cplusplus
}
#endif
#endif//BROADCAST_IP_ADDR_CLENT_H

