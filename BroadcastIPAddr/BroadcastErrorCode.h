#ifndef BROADCAST_ERROR_CODE_H
#define BROADCAST_ERROR_CODE_H
enum BroadcastErrorCode
{
	BEC_SUCCESS = 0x0,
	//network erro
	BEC_FAIL_BIND_SOCKET,		//bind socket Fail
	BEC_FAIL_SET_SOCKETOPT,		//set socketopt Fail
	BEC_FAIL_SEND_DATA,			//send data Fail
	BEC_FAIL_REC_DATA,			//receive data Fail
	BEC_FAIL_GET_HOST_NAME,		//get host name Fail
	BEC_FAIL_GET_HOST_IP,		//get host IP Fail

	//parameter error
	BEC_FAIL_PTR_IS_NULL,		//A pointer is empty



};
#endif // BROADCAST_ERROR_CODE_H