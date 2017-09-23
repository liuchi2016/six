#ifndef _tcpmoduledefine_h_20150606_lc_
#define _tcpmoduledefine_h_20150606_lc_

#include <string>
#include <boost/function.hpp>
#include "stdint.h"

enum notify_type
{
	disconnect,
	connected
};

struct ID
{
	std::string ip;
};

#endif