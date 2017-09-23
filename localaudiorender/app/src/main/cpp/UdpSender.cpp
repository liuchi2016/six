#include "UdpSender.h"
#include <common/stringexcept.h>

void   UdpSender::Initialize(short m_port)
{
	port = m_port;
	Create();
}

void   UdpSender::Send(std::string message, std::string sIp, int iSendPort)
{
	int  addr_len  = sizeof(struct sockaddr);

	struct	sockaddr_in remote;
	remote.sin_family = AF_INET;

	remote.sin_port = htons(iSendPort);

	remote.sin_addr.s_addr = inet_addr(sIp.c_str());

	sendto(sck,message.c_str(), message.size(), 0,(struct sockaddr *)&remote,addr_len);
}

void   UdpSender::Create()
{
	sck = socket(AF_INET, SOCK_DGRAM, 0);

	if (sck == INVALID_SOCKET)
		throw string_exception("create failed:");

	struct	sockaddr_in local;
	local.sin_family = AF_INET; 
	local.sin_port = htons(port); 
	local.sin_addr.s_addr = htonl(INADDR_ANY);

	if(::bind(sck, (struct sockaddr*)&local, sizeof(local)) == SOCKET_ERROR){
		close(sck);
		sck = INVALID_SOCKET;
		throw string_exception("bind failed:");
	} 

	int nSendBuf = 5 * 1024 * 1024;

	setsockopt(sck,SOL_SOCKET, SO_SNDBUF, (const char *)&nSendBuf,sizeof(nSendBuf));
}

void   UdpSender::UnInitialize()
{
	if (sck  != INVALID_SOCKET) 
	{
		close(sck); 
		sck = INVALID_SOCKET;
	}
}


