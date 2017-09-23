#ifndef _STRING_EXCEPTION_H_2015_05_30_LC_
#define _STRING_EXCEPTION_H_2015_05_30_LC_

#include <string>
#include <exception>

class   string_exception : public  std::exception
{
public:
	string_exception(std::string  _sdesc){sdescription = _sdesc;}
	virtual const char* what() const throw()
	{
		return  sdescription.c_str();
	}
	bool  quit(){
		return sdescription == std::string("quit");
	}
	~string_exception()throw (){}
	std::string  get()
	{
		return sdescription;
	}
private:
	std::string sdescription;

};

#endif