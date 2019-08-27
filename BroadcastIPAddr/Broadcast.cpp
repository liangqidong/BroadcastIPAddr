#include "Broadcast.h"

#include <regex> 

#define RecBuffLen  1024
#define RecTimeOut  100 //ms

BroadcastSocket::BroadcastSocket()
{
	m_recDataFun = nullptr;
	m_recDataThread = nullptr;
	m_threadQuit = false;
}


BroadcastSocket::~BroadcastSocket()
{
	m_threadQuit = true;
	if (nullptr != m_recDataThread && m_recDataThread->joinable())
	{
		m_recDataThread->join();
		delete m_recDataThread;
		m_recDataThread = nullptr;
	}
	
}

long BroadcastSocket::SendData(const char* data, unsigned int& dataLen, const char* addrIP)
{
	
	std::regex regex = std::regex("^(25[0-5]|2[0-4]\\d|[0-1]\\d{2}|[1-9]?\\d)\\.(25[0-5]|2[0-4]\\d|[0-1]\\d{2}|[1-9]?\\d)\\.(25[0-5]|2[0-4]\\d|[0-1]\\d{2}|[1-9]?\\d)\\.(25[0-5]|2[0-4]\\d|[0-1]\\d{2}|[1-9]?\\d)$");
	if (!std::regex_match(addrIP, regex))
	{
		return -1;
	}

	unsigned long addr = inet_addr(addrIP);
	regex = std::regex("^([0])\\.([0])\\.([0])\\.([0])$");
	if (std::regex_match(addrIP, regex))
	{
		addr = INADDR_BROADCAST;
	}
	return m_udpSocket.SendData(data, dataLen, addr);
}

long BroadcastSocket::RegRecFun(BroadcastRecDataFun recDataFun)
{
	m_recDataFun = recDataFun;
	return 0;
}

void BroadcastSocket::RecDataThread()
{
	char buff[RecBuffLen] = { 0 };
	unsigned int dataLen = RecBuffLen;
	char ip[MAX_PATH] = { 0 };
	unsigned int port = 0;
	unsigned int ipBuffLen = MAX_PATH;
	while (!m_threadQuit)
	{
		if (-1 == m_udpSocket.RecvData(buff, dataLen, RecTimeOut))
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			continue;
		}
		if (nullptr == m_recDataFun)
		{
			continue;
		}
		m_udpSocket.GetSocketIP(ip, ipBuffLen);
		m_udpSocket.GetSocketPort(port);
		m_recDataFun(ip, port, buff, dataLen);
		memset(buff, 0, RecBuffLen);
		memset(ip, 0, MAX_PATH);
		dataLen = RecBuffLen;
	}
}

long BroadcastSocket::BindReceivePort(unsigned int port)
{
	m_udpSocket.SetReceivePort(port);
	return 0;
}

long BroadcastSocket::bindSendPort(unsigned int port)
{
	m_udpSocket.SetSendPort(port);
	return 0;
}

long BroadcastSocket::Process()
{
	if (nullptr != m_recDataThread)
	{
		return 0;
	}
	m_recDataThread = new std::thread(&BroadcastSocket::RecDataThread, this);
}

long BroadcastSocket::getUDPSocket(UDPSocket** udpSocket)
{
	//udpSocket = &m_udpSocket;
	return 0;
}

long BroadcastSocket::getThreadobj(std::thread** tThreadobj)
{
	//tThreadobj = &m_recDataThread;
	return 0;
}
