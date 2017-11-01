#ifndef _DATA_LOGIC_H_2014_10_27_LC_
#define _DATA_LOGIC_H_2014_10_27_LC_

#include <boost/thread.hpp>
#include "tcpmoduledefine.h"
class   DataLogic
{
public:
	void    NotifyFromClientServerLib(ID id,notify_type type,std::string externs);
	void	GetDataFromTeacher(ID,uint8_t* buf,int length);
	void 	SetValue(std::string studentIp,std::string smac);
	std::string _studentIp;
	std::string _smac;
};
#endif
