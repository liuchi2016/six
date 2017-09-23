#ifndef  _WAIT_WRAPPER_H_2014_05_06_LC_
#define  _WAIT_WRAPPER_H_2014_05_06_LC_

#include <unistd.h>

struct  WaitWrapper
{
	static    void   wait(int  millsecs)
	{
		usleep(millsecs * 1000);
	}
};

#endif
