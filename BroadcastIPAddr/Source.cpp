#include "BroadcastServer.h"
#include "BroadcastClient.h"
#include <iostream>
#include <windows.h>

#define Client 

using namespace std;
BroadcastServer BS;
BroadcastClient BC;
long BroadcastDataFun(const char* ip,unsigned port, const char* data, unsigned int dataLen)
{
	std::cout << "IP:" << ip << std::endl;
	std::cout << "port:" << port << std::endl;
	std::cout << "data:" << data << std::endl;
	std::cout << "dataLen:" << dataLen << std::endl;

#ifdef Client
	unsigned int len = sizeof("GetIPAddr");
	BC.SendData("GetIPAddr", len, ip);
#else
	unsigned int len = strlen("GetIPAddr");
	BS.SendData("GetIPAddr", len, ip);
#endif

	return 0;
}

int main()
{
#ifdef Client
	//client
	BC.RegRecFun(std::bind(&BroadcastDataFun, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	BC.bindSendPort(12811);
	unsigned int len = sizeof("GetIPAddr");
	BC.Process();
	BC.SendData("GetIPAddr", len);

#else
//server
	BS.RegRecFun(std::bind(&BroadcastDataFun,std::placeholders::_1,std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	BS.BindReceivePort(12811);
	BS.Process();
	unsigned int len = sizeof("GetIPAddr");
	BS.SendData("GetIPAddr", len);
#endif
	while (true)
	{
		Sleep(100);
	}
	return 0;
}
