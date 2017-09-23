#ifndef _reciever_h_20150828_lc
#define _reciever_h_20150828_lc
#include <string>
#include <boost/function.hpp>
#include <common/stringexcept.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "arpa/inet.h"
#include <common/types.h>

typedef boost::function<void(uint8_t*,int)>  CallBackType;

class Reciever
{
public:
	void UnInitialize();
	void Initialize(std::string remoteIp,short remotPort, short VerityPort);
	void DataLoop(CallBackType _call);
private:
	bool IsMultiCastIp(std::string);
	void Create();
	void Close();
    void Verfity(std::string remote, short port);
private:
	SOCKET			m_socket;
	std::string		remoteIp;
	short			remotPort;
	short 			verityPort;
};



#endif