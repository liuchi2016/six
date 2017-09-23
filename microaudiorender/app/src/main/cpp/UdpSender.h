#ifndef  _UdpSender_h_20150828_lc_
#define  _UdpSender_h_20150828_lc_

#include <sys/socket.h>
#include <netinet/in.h>
#include "arpa/inet.h"

#include <common/WinError.h>
#include <string>
#include <set>
#include <vector>

#include <common/types.h>

class  UdpSender
{
public:
	void   Initialize(short);

	void   UnInitialize();

	void   Send(std::string message, std::string sIp, int iSendPort);
private:
	void   Create();
private:
	SOCKET  sck;
	short			port;
};
#endif