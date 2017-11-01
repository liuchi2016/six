#include "DataLogic.h"
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>
#include "common/DataCacheWrapper.h"
#include "gloab.h"
#include "common/MessageDefine.h"
#include "common/ParseWrapper.h"

void DataLogic::NotifyFromClientServerLib(ID id,notify_type type,std::string externs)
{
	if (type  == disconnect)
	{
		GetApp()->Java().Log("disconnected");
		GetApp()->Java().Stop("screen");
        GetApp()->Java().Stop("mac");
        GetApp()->Java().Stop("audio");
        GetApp()->Java().Stop("techvideo");
	}

	else if (type == connected)
	{
        GetApp()->Java().Log("connected");
        Cache<1024>  cache;
        Header  header;
        header.id  = MAC;
        header.operation  = 0;
        header.type    = 0;
        std::string sdata  = "/";
        ParseWrapper::IntegrateA(externs,sdata);
        ParseWrapper::IntegrateA(_smac,sdata);
        ParseWrapper::IntegrateA(_studentIp,sdata);
        header.externeds  = sdata.size();
        cache.write(header);
        cache.write((uint8_t*)sdata.c_str(),sdata.size());
        GetApp()->GetTcpClient().Send(cache.get(),cache.size());
    }
}

void DataLogic::GetDataFromTeacher(ID,uint8_t* buf,int length)
{
	Header  header  =   *(Header*)buf;

	::Classification  id  =  (::Classification)(header.id);

	if (id == MULTIAUDIO)
	{
		if (header.operation)
		{
			char smultiip[128] = {0};
			memcpy(smultiip,buf + sizeof(header),header.externeds);
			std::string ip = ParseWrapper::ParseA(smultiip,0);
			short  port = boost::lexical_cast<short>(ParseWrapper::ParseA(smultiip,1));
            short  udpVerityPort = boost::lexical_cast<short>(ParseWrapper::ParseA(smultiip,2));
			switch(header.type)
			{
			case 0:
				{
                    GetApp()->Java().Start("mac",ip,port,udpVerityPort);
				}
				break;
			case 1:
				{
                    GetApp()->Java().Start("audio",ip,port,udpVerityPort);
				}
				break;
			}
		}
		else
		{
			switch(header.type)
			{
			case 0:
				{
                    GetApp()->Java().Stop("mac");
				}
				break;
			case 1:
				{
                    GetApp()->Java().Stop("audio");
				}
				break;
			}
		}
	}
	else if (id == MULTICASTFULLSCREEN)
	{
        GetApp()->Java().Set("fullscreen",header.operation);
	}

	else if (id == VIDEOTECH)
	{
		if (!header.operation)
		{
			char smultiip[128] = {0};
			memcpy(smultiip,buf + sizeof(header),header.externeds);
			std::string ip = ParseWrapper::ParseA(smultiip,0);
			short  port = boost::lexical_cast<short>(ParseWrapper::ParseA(smultiip,1));
            GetApp()->Java().Start("techvideo",ip,port,0);
		}
		else
		{
            GetApp()->Java().Stop("techvideo");
		}
	}
	else if (id == MULTICASTSCREEN || id == MULTICASTSTUDENTDEMORECIEVOR)
	{
		if (header.operation == 0)
		{
			char smultiip[128] = {0};
			memcpy(smultiip,buf + sizeof(header),header.externeds);
			std::string ip = ParseWrapper::ParseA(smultiip,0);
			short  port = boost::lexical_cast<short>(ParseWrapper::ParseA(smultiip,1));
            std::string compress = ParseWrapper::ParseA(smultiip,2);
            short  udpVerityPort = boost::lexical_cast<short>(ParseWrapper::ParseA(smultiip,3
            ));
            GetApp()->Java().Start("screen",ip,port,udpVerityPort);
		}
		else if (header.operation == 1 || header.operation == 2)
		{
            GetApp()->Java().Stop("screen");
		}
	}
}

void DataLogic::SetValue(std::string studentIp,std::string smac){
	this->_studentIp = studentIp;
	this->_smac = smac;
}