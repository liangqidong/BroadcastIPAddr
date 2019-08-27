#include "BroadcastIPAddrClient.h"
#include "BroadcastClient.h"
#include "BroadcastErrorCode.h"

//构造对象
EXPORTDLL_API void CreateClientObj(BroadcastIPAddrClient* obj)
{
	obj = new BroadcastIPAddrClient;
}
//析构对象
EXPORTDLL_API void DestroyClientObj(BroadcastIPAddrClient* obj)
{
	if (nullptr != obj)
	{
		delete obj;
		obj = nullptr;
	}
}

BroadcastIPAddrClient::BroadcastIPAddrClient()
	: obj(nullptr)
{
	obj = new BroadcastClient();
}


BroadcastIPAddrClient::~BroadcastIPAddrClient()
{
	BroadcastClient* object = (BroadcastClient*)obj;
	if (nullptr != object)
	{
		delete object;
		object = nullptr;
	}
}

long BroadcastIPAddrClient::RegRecFun(BroadcastRecDataFun recDataFun)
{
	if (nullptr == obj)
	{
		return BEC_FAIL_PTR_IS_NULL;
	}
	return BroadcastClientObj(obj)->RegRecFun(recDataFun);
}

long BroadcastIPAddrClient::BindBroadcastServerPort(unsigned int port)
{
	if (nullptr == obj)
	{
		return BEC_FAIL_PTR_IS_NULL;
	}
	return BroadcastClientObj(obj)->bindSendPort(port);
}

long BroadcastIPAddrClient::SendData(const char* data, unsigned int& dataLen, const char* addrIP )
{
	if (nullptr == obj)
	{
		return BEC_FAIL_PTR_IS_NULL;
	}
	return BroadcastClientObj(obj)->SendData(data, dataLen, addrIP);
}

long BroadcastIPAddrClient::Process()
{
	if (nullptr == obj)
	{
		return BEC_FAIL_PTR_IS_NULL;
	}
	return BroadcastClientObj(obj)->Process();
}
