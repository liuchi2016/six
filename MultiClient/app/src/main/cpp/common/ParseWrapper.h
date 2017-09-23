#ifndef   _PARSE_WRAPPER_H_2014_05_08_LC_
#define _PARSE_WRAPPER_H_2014_05_08_LC_

#include <string>
#include <boost/lexical_cast.hpp>


struct  ParseWrapper
{
	static  std::wstring  Parse(std::wstring params,int  index,std::wstring sp = L"/")
	{
		int count = 0,start  =  0,end   =  0;
		std::wstring src;
		do 
		{
			start  =   params.find(sp,start);
			start +=1;
			++count; 
		} while (count != (index + 1));
		end  =  params.find(sp,start);
		src  =  params.substr(start,end - start);
		return  src;
	}
	template<typename T> static  void Integrate(T t,std::wstring& s,std::wstring sp = L"/")
	{
		s += boost::lexical_cast<std::wstring>(t);
		s +=sp;
	}

	static  std::string  ParseA(std::string params,int  index,std::string sp = "/")
	{
		int count = 0,start  =  0,end   =  0;
		std::string src;
		do 
		{
			start  =   params.find(sp,start);
			start +=1;
			++count; 
		} while (count != (index + 1));
		end  =  params.find(sp,start);
		src  =  params.substr(start,end - start);
		return  src;
	}

	template<typename T> static  void IntegrateA(T t,std::string& s,std::string sp = "/")
	{
		s += boost::lexical_cast<std::string>(t);
		s += sp;
	}

};

#endif // _DEBUG
