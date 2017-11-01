#ifndef _tcp_client_h_20150606_lc_
#define _tcp_client_h_20150606_lc_

#include <boost/bind.hpp>
#include <unistd.h>
#include "tcpmoduledefine.h"
#include "common/WaitWrapper.h"
#include "common/netutility.h"
#include "common/DataCacheWrapper.h"

class tcpclient
{
public:
	void SetRemoteIp(std::string host);
	void Run(short port);
	void Send(uint8_t* buf,int length);
private:
	virtual void deal_packet(ID id,uint8_t* buf,int length); 
	void  Close();
	void  worker();
	bool  ReceiveOnPackage(int length);
	bool  _connect();
	bool  _Send(uint8_t* buf,int length);
private:
	ID        id;
	short     port;
	bool      bConnected;
	SOCKET    m_socket;
	std::string  host;
};

#endif
