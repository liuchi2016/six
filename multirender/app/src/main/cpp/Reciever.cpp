#include "Reciever.h"
#include <boost/format.hpp>
#include <common/WinError.h>
#include <boost/thread.hpp>
#include "UdpSender.h"

#define  THROWLASTERROR(prefix)  \
	boost::format fmt("%s:%s"); \
	fmt % prefix % ResolveErrorA();	\
	throw string_exception(fmt.str());


void   Reciever::Initialize(std::string remoteIp,short remotPort, short VerityPort)
{
	this->remoteIp = remoteIp;
	this->remotPort = remotPort;
    this->verityPort = VerityPort;
	m_socket = INVALID_SOCKET;
	Create();
}

void  Reciever::UnInitialize(){
	Close();
}

void  Reciever::Create()
{
	m_socket = socket(AF_INET, SOCK_DGRAM, 0);

	if (m_socket == INVALID_SOCKET ){
		THROWLASTERROR("socket func");
	}
	struct	sockaddr_in local;
	local.sin_family = AF_INET; 
	local.sin_port = htons(remotPort); 
	local.sin_addr.s_addr = htonl(INADDR_ANY);
	if(::bind(m_socket, (struct sockaddr*)&local, sizeof(local)) == SOCKET_ERROR){
		THROWLASTERROR("bind func");
	} 

	int nRecvBuf = 5 * 1024 * 1024;

	setsockopt(m_socket,SOL_SOCKET, SO_RCVBUF, (const char *)&nRecvBuf,sizeof(nRecvBuf));

	if (IsMultiCastIp(remoteIp)){
		struct	ip_mreq mcast;
		mcast.imr_multiaddr.s_addr = inet_addr(remoteIp.c_str()); 
		mcast.imr_interface.s_addr = htonl(INADDR_ANY);
		if(setsockopt(m_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mcast, sizeof(mcast)) == SOCKET_ERROR) { 
			THROWLASTERROR("joined membership:");
		}
	} else{
        boost::thread(boost::bind(&Reciever::Verfity,this,remoteIp,verityPort));

    }
}


void Reciever::Verfity(std::string remote, short port){
    UdpSender sender;
    sender.Initialize(port+50);
    while (true){
        sender.Send("coming in",remote,port);
        usleep(1000000);
    }
    sender.UnInitialize();
}


void   Reciever::DataLoop(CallBackType _call)
{
	char data[1472] = {0};
	while(true){
			int  addr_len  = sizeof(struct sockaddr);
			struct sockaddr   addr;
			int rbytes=recvfrom(m_socket,data,1472,0,(struct sockaddr*)&addr,&addr_len);
			if (rbytes == SOCKET_ERROR){
				THROWLASTERROR("recvfrom");
			}
			else{
				_call((uint8_t*)data,rbytes);
			}
		}
}

bool Reciever::IsMultiCastIp(std::string ip)
{
	std::string::size_type pos = ip.find("229.");
	return pos != std::string::npos;
}

void Reciever::Close(){
	if (m_socket  != INVALID_SOCKET){
		close(m_socket);
		m_socket = INVALID_SOCKET;
	}
}





