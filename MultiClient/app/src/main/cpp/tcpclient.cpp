#include "unistd.h"
#include "tcpclient.h"
#include "gloab.h"
#include "common/SafetyDelete.h"

#define   PerSendBufferBytes   8192


void tcpclient::SetRemoteIp(std::string host){
    this->host = host;
}

bool tcpclient::_connect() {

	if (this->host.empty()) {
		return  false;
	}

	if (!bConnected)
	{
		id.ip = this->host;
		sockaddr_in  addr;
		memset(&addr,0,sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_addr.s_addr= inet_addr(id.ip.c_str());
		addr.sin_port = htons(port);
		m_socket = socket(AF_INET,SOCK_STREAM,0);
		if (m_socket == INVALID_SOCKET) {
            return false;
		}

		if (connect(m_socket,(sockaddr*)&addr,sizeof(addr)))
		{
			close(m_socket);
			m_socket  = INVALID_SOCKET;
		}
		else
		{
			int   bKeepAlive = 1;
			setsockopt(m_socket, SOL_SOCKET, SO_KEEPALIVE, (void*)&bKeepAlive, sizeof(bKeepAlive));
			int                 keepIdle = 2;
			int                 keepInterval = 1;
			int                 keepCount = 2;
			setsockopt(m_socket, SOL_TCP, TCP_KEEPIDLE, (void *)&keepIdle, sizeof(keepIdle));
			setsockopt(m_socket, SOL_TCP, TCP_KEEPINTVL, (void *)&keepInterval, sizeof(keepInterval));
			setsockopt(m_socket, SOL_TCP, TCP_KEEPCNT, (void *)&keepCount, sizeof(keepCount));

			bConnected = true;
			sockaddr_in  local;
			socklen_t  length  =  sizeof(sockaddr_in);
			getsockname(m_socket,(struct sockaddr*)&local,&length);
			GetApp()->Logic().NotifyFromClientServerLib(id,connected,inet_ntoa(local.sin_addr));
		}
	}
	return  bConnected;
}

bool tcpclient::ReceiveOnPackage(int length)
{
	int  dwRecieveBytes = 0;
	uint8_t*  buf = new uint8_t[length];
	bool  bSuccessed  = true;
	while(length > dwRecieveBytes)
	{ 
		int  nCurRecvBytes = (length - dwRecieveBytes) >= PerSendBufferBytes ? PerSendBufferBytes :(length - dwRecieveBytes);
		if ((nCurRecvBytes = recv(m_socket,(reinterpret_cast<char*>(buf)) + dwRecieveBytes,nCurRecvBytes,0)) >=0 )
		{
			dwRecieveBytes += nCurRecvBytes;
		}
		else if(nCurRecvBytes == SOCKET_ERROR)
		{
			bSuccessed  = false;
			break;
		}
	}
	if (length == dwRecieveBytes)
	{
		deal_packet(id,buf,length);
	}

	SafeDeleteWrapper::SafeArrayDelete(buf);

	return  bSuccessed;
}


void tcpclient::worker()
{
	while(true) {
		if (_connect()) {
			timeval   waiter;
			waiter.tv_sec = 3;
			waiter.tv_usec = 0;
			fd_set   revicors;
			FD_ZERO(&revicors);
			FD_SET(m_socket,&revicors);
			if (select(m_socket + 1,&revicors,0,0,&waiter) > 0
                && FD_ISSET(m_socket,&revicors)) {
				int      length = 0;
				int  bytesRecv  =  recv(m_socket,(char*)&length,sizeof(length),0);
				if (bytesRecv <= 0
                    || ! ReceiveOnPackage(length)) {
					Close();
				}
			}
		}
		else {
			WaitWrapper::wait(2000);
		}
	}
}

bool tcpclient::_Send(uint8_t* buf,int length)
{
	bool   bSuccessed  =  true;

	int  dwSendTotals = 0;

	while(length >  dwSendTotals) {
		int  nCurSendBytes = (length - dwSendTotals) >= PerSendBufferBytes ? PerSendBufferBytes :(length - dwSendTotals);

		if ((nCurSendBytes = send(m_socket,(reinterpret_cast<char*>(buf)) + dwSendTotals,nCurSendBytes,0)) >=0 ) {
			dwSendTotals += nCurSendBytes;
		}
		else if(nCurSendBytes == SOCKET_ERROR) {
			bSuccessed   =  false;
			break;
		}
	}
	return   bSuccessed;
}


void tcpclient::Run(short port) {
	this->port = port;
    bConnected = false;
	worker();
}


void tcpclient::deal_packet( ID id,uint8_t* buf,int length )
{
	GetApp()->Logic().GetDataFromTeacher(id,buf,length);
}


void tcpclient::Close() {
	GetApp()->Logic().NotifyFromClientServerLib(id,disconnect,"");
	bConnected  = false;
	if (m_socket != INVALID_SOCKET) {
		close(m_socket);
		m_socket = INVALID_SOCKET;
	}
}

void  tcpclient::Send(uint8_t* buf,int length) {
	if (bConnected) {
		DataCache* cache  = new DataCache(buf,length);
		_Send(cache->get(),cache->size());
		SafeDeleteWrapper::SafeDelete(cache);
	}
}
