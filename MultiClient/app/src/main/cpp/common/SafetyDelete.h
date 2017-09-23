#ifndef _SAFETY_DELETE_H_2014_05_05_LC
#define _SAFETY_DELETE_H_2014_05_05_LC
#include <exception>
struct   SafeDeleteWrapper
{
	template<typename   T> static  void SafeDelete(T*& p)
	{
		try
		{
			if (p)
			{
				delete  p;
				p = 0;
			}
		}
		catch(std::exception&)
		{
		}
	}

	template<typename   T> static  void SafeArrayDelete(T*& p)
	{

		try
		{
			if (p)
			{
				delete[]  p;
				p = 0;
			}
		}
		catch(std::exception&)
		{
		}
	}

	template<typename   T> static  void SafeRelease(T*& p)
	{
		if (p)
		{
			p->Release();
			p = 0;
		}
	}
};

#endif
