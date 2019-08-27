#include "BroadcastIPAddrServer.h"
#include "BroadcastServer.h"
#include "BroadcastErrorCode.h"

BroadcastIPAddrServer::BroadcastIPAddrServer()
	: obj(nullptr)
{
	obj = new BroadcastServer();
}

BroadcastIPAddrServer::~BroadcastIPAddrServer()
{
	BroadcastServer* object = (BroadcastServer*)obj;
	if (nullptr != object)
	{
		delete object;
		object = nullptr;
	}
}

long BroadcastIPAddrServer::RegRecFun(BroadcastRecDataFun recDataFun)
{
	if (nullptr == obj)
	{
		return BEC_FAIL_PTR_IS_NULL;
	}
	return BroadcastServerObj(obj)->RegRecFun(recDataFun);
}

long BroadcastIPAddrServer::BindBroadcastReceivePort(unsigned int port)
{
	if (nullptr == obj)
	{
		return BEC_FAIL_PTR_IS_NULL;
	}
	return BroadcastServerObj(obj)->BindReceivePort(port);
}

long BroadcastIPAddrServer::SendData(const char* data, unsigned int& dataLen, const char* addrIP /*= "0.0.0.0"*/)
{
	if (nullptr == obj)
	{
		return BEC_FAIL_PTR_IS_NULL;
	}
	return BroadcastServerObj(obj)->SendData(data, dataLen, addrIP);
}

long BroadcastIPAddrServer::Process()
{
	if (nullptr == obj)
	{
		return BEC_FAIL_PTR_IS_NULL;
	}
	return BroadcastServerObj(obj)->Process();
}

EXPORTDLL_API void CreateServerObj(BroadcastIPAddrServer* obj)
{
	obj = new BroadcastIPAddrServer;
}

EXPORTDLL_API void DestroyServerObj(BroadcastIPAddrServer* obj)
{
	if (nullptr != obj)
	{
		delete obj;
		obj = nullptr;
	}
}
