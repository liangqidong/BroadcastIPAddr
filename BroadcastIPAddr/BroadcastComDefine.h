#ifndef BROADCAST_COMMON_DEFINITION_H
#define BROADCAST_COMMON_DEFINITION_H


#include <functional>

#ifdef EXPORTDLL_EXPORTS
#define EXPORTDLL_API __declspec(dllexport)
#else
#define EXPORTDLL_API __declspec(dllimport)
#endif

#ifndef BroadcastRecDataFun
#define BroadcastRecDataFun std::function<long(const char* ip,unsigned int& port, const char* data, unsigned int dataLen)> 
#endif//BroadcastRecDataFun

#ifndef BroadcastClientObj
#define BroadcastClientObj(a) ((BroadcastClient*)a)
#endif//BroadcastClientObj

#ifndef BroadcastServerObj
#define BroadcastServerObj(a) ((BroadcastServer*)a)
#endif//BroadcastServerObj


//参数定义
#define RecBuffLen  1024
#define RecTimeOut  100 //ms

#ifndef REGEX_CHECK_IP
#define REGEX_CHECK_IP "^(25[0-5]|2[0-4]\\d|[0-1]\\d{2}|[1-9]?\\d)\\.(25[0-5]|2[0-4]\\d|[0-1]\\d{2}|[1-9]?\\d)\\.(25[0-5]|2[0-4]\\d|[0-1]\\d{2}|[1-9]?\\d)\\.(25[0-5]|2[0-4]\\d|[0-1]\\d{2}|[1-9]?\\d)$"
#endif//REGEX_CHECK_IP

#ifndef REGEX_CHECK_0_0_0_0
#define REGEX_CHECK_0_0_0_0 "^([0])\\.([0])\\.([0])\\.([0])$"
#endif//REGEX_CHECK_0_0_0_0

#endif // BRODCAST_COMMON_DEFINITION_H
