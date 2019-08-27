#include "Broadcast.h"
#include <regex> 
#include "BroadcastErrorCode.h"
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
	
	std::regex regex = std::regex(REGEX_CHECK_IP);
	if (!std::regex_match(addrIP, regex))
	{
		return -1;
	}

	unsigned long addr = inet_addr(addrIP);
	regex = std::regex(REGEX_CHECK_0_0_0_0);
	if (std::regex_match(addrIP, regex))
	{
		addr = INADDR_BROADCAST;
	}
	return m_udpSocket.SendData(data, dataLen, addr);
}

long BroadcastSocket::RegRecFun(BroadcastRecDataFun recDataFun)
{
	m_recDataFun = recDataFun;
	return BEC_SUCCESS;
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
	return m_udpSocket.SetReceivePort(port);;
}

long BroadcastSocket::bindSendPort(unsigned int port)
{
	return m_udpSocket.SetSendPort(port);;
}

long BroadcastSocket::Process()
{
	if (nullptr != m_recDataThread)
	{
		return BEC_FAIL_PTR_IS_NULL;
	}
	m_recDataThread = new std::thread(&BroadcastSocket::RecDataThread, this);
	return BEC_SUCCESS;
}

long BroadcastSocket::getUDPSocket(UDPSocket** udpSocket)
{
	//udpSocket = &m_udpSocket;
	return BEC_SUCCESS;
}

long BroadcastSocket::getThreadobj(std::thread** tThreadobj)
{
	//tThreadobj = &m_recDataThread;
	return BEC_SUCCESS;
}
