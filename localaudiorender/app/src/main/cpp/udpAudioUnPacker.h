#ifndef _UDP_AUDIO_UNPACKER_H_2017_03_27_LC_
#define _UDP_AUDIO_UNPACKER_H_2017_03_27_LC_

#include "DataQueue.h"
#include <common/AudioUdpDefine.h>
#include <common/DataCacheWrapper.h>
#include "aacdec.h"

class UdpAudioUnpacker{
public:
	UdpAudioUnpacker();
	typedef SimpleCache		BufferCls;
	typedef DataQueue<BufferCls>::Element  ElementCls;
	typedef DataQueue<BufferCls>  QueueCls;
	void  Flush(uint8_t* buf ,int size);
	void  Get(uint8_t*& out,int& out_len);
private:
	UdpAudioUnpacker::ElementCls pop();
public:
	QueueCls  queues;
	AACDecoder	decoder;
};


#endif

