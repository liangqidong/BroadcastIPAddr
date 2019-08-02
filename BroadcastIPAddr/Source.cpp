#include "UDPSocket.h"
#include <iostream>
#include <windows.h>

using namespace std;
int main()
{
// 	//client
// 	UDPSocket us;
// 	us.SetBindPort(11121);
// 	us.SetReceivePort(12811);
// 	us.bindSocket();
// 	unsigned int len = sizeof("GetIPAddr");
// 	us.SendData("GetIPAddr", len);
// 	char buff[MAX_PATH] = { 0 };
// 	len = MAX_PATH;
// 	us.RecvData(buff, len);

	//server
	UDPSocket us;
	UDPSocket::IPInfo ipInfo;
	us.GetLocalIP(ipInfo.ip);

	us.SetBindPort(12811);
	us.bindSocket();
	char buff[MAX_PATH] = { 0 };
	unsigned int len = MAX_PATH;
	while (true)
	{
		us.RecvData(buff, len);
		if (0 == len)
		{
			Sleep(10);
		}
		else
		{
			printf("%s\n", buff);
			break;
		}
		
	}
	
	len = sizeof("GetIPAddr");
	us.SendData("GetIPAddr", len);
	return 0;
}