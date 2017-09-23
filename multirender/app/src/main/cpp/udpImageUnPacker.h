//
// Created by admin on 17-6-5.
//
#ifndef MULTIREDIRECTIONCLIENT_UDPIMAGEUNPACKER_H
#define MULTIREDIRECTIONCLIENT_UDPIMAGEUNPACKER_H

#include <set>
#include "DataQueue.h"
#include <common/DataCacheWrapper.h>
#include <common/stringexcept.h>
class  VideoFrame
{
private:
    static const int  mtu_value = 1400;
    uint16_t	frames;
    uint32_t    size;
    uint16_t	id;
    uint16_t	wCount;
    uint8_t*	_data;
public:
    VideoFrame();
    ~VideoFrame();
    bool		Flush(uint8_t* buf,int length,uint16_t index);
    uint16_t	GetId();
    void		Reset(uint16_t frames,uint16_t id);
    uint8_t*	Data();
    uint32_t	Size();
};


class UdpImageUnpacker{
public:
    typedef SimpleCache		BufferCls;
    typedef DataQueue<BufferCls>::Element  ElementCls;
    typedef DataQueue<BufferCls>  QueueCls;
    void    Flush(uint8_t* buf ,int size);
    void    Stop();
    UdpImageUnpacker();
private:
    UdpImageUnpacker::ElementCls pop();
private:
    static const int  frame_max_size = 100;
    VideoFrame  frames[frame_max_size];
public:
    bool  bStop;
    QueueCls  queues;
    short width;
    short height;
};

#endif //MULTIREDIRECTIONCLIENT_UDPIMAGEUNPACKER_H
