#ifndef  _WAIT_WRAPPER_H_2014_05_06_LC_
#define _WAIT_WRAPPER_H_2014_05_06_LC_

#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
//#include <Windows.h>

struct  WaitWrapper
{
	static    void   wait(int  millsecs)
	{
		boost::this_thread::sleep(boost::posix_time::microsec(millsecs));
	}
};

#endif
