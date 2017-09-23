#ifndef _DATACACHE_WRAPPER_H_2014_05_14_LC_
#define _DATACACHE_WRAPPER_H_2014_05_14_LC_
#include "SafetyDelete.h"
#include <stdint.h>
#include <string>
#include "string.h"
#include <fstream>

template<int N>
class  Cache
{
public:
	Cache()
	{
		data = new uint8_t[N];
		cur = data;
		length = 0;
	}
	~Cache()
	{
		SafeDeleteWrapper::SafeDelete(data);
	}
private:
	uint8_t* data;
	uint8_t* cur;
	int      length;
public:
	template<typename T> void   write(T& ptr)
	{
		memcpy(cur,&ptr,sizeof(T));
		cur    += sizeof(T);
		this->length += sizeof(T);
	}

	void   write(uint8_t* buf,int length)
	{
		memcpy(cur,buf,length);
		cur    += length;
		this->length += length;
	}

	uint8_t*  get()
	{
		return  data;
	}

	int    size()
	{
		return  length;
	}
};


class  DataCache
{
private:
	int   length;
	uint8_t* buf;
public:
	DataCache(uint8_t* rgb,int _length) :buf(0)
	{
		length  =  _length;
		buf = new uint8_t[length + sizeof(int)];
		memcpy(buf,(char*)&length,sizeof(int));
		memcpy(buf + sizeof(int),rgb,length);
	}

	DataCache(int  fixbytes)
	{
		buf = new uint8_t[fixbytes];
		length  =  0;
	}
	uint8_t*&   get(){return  buf;}
	uint8_t*     data(){return buf + sizeof(int);}
	int    len(){return  length;}
	int    size(){return length + sizeof(int);}
	void  copy(uint8_t* rgb,int _length)
	{
		length  = _length;
		memcpy(buf,(char*)&length,sizeof(int));
		memcpy(buf + sizeof(int),rgb,length);
	}
	~DataCache()
	{
		SafeDeleteWrapper::SafeDelete(buf);
	}
};

#endif
