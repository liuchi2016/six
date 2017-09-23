#include "udpImageUnPacker.h"
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

struct  ImageUdpHeader
{
	uint16_t          index;
	uint16_t		  frames;
	uint16_t		  width;
	uint16_t		  heigth;
	uint16_t		  frame_id;
};

void VideoFrame::Reset(uint16_t frames,uint16_t id)
{
	this->id = id;
	size = 0;
	this->frames = frames;
	this->wCount = 0;
}

VideoFrame::VideoFrame()
{
	_data = new uint8_t[1024 * 1024];
	id = -1;
}

bool VideoFrame::Flush(uint8_t* buf,int length,uint16_t index){
	memcpy(_data + index * mtu_value,buf,length);
	size += length;
	wCount++;
	return wCount == frames;
}

VideoFrame::~VideoFrame(){
	SafeDeleteWrapper::SafeDelete(_data);
}

uint16_t VideoFrame::GetId(){
	return  id;
}

uint8_t* VideoFrame::Data()
{
	return _data;
}

uint32_t VideoFrame::Size()
{
	return size;
}

void UdpImageUnpacker::Flush(uint8_t* buf ,int size){
	if (bStop){
		throw string_exception("quit");
	}
	if (size > sizeof(ImageUdpHeader)){
		ImageUdpHeader& hdr = *(ImageUdpHeader*)buf;
		VideoFrame&  frame = frames[hdr.frame_id%frame_max_size];
        if(width != hdr.width || height != hdr.heigth){
            width = hdr.width;
            height = hdr.heigth;
        }
		if(frame.GetId() != hdr.frame_id)
			frame.Reset(hdr.frames,hdr.frame_id);
		if (frame.Flush(buf + sizeof(ImageUdpHeader),size - sizeof(ImageUdpHeader),hdr.index)){
			queues.Push(new BufferCls(frame.Data(),frame.Size()));
		}
	}
}

UdpImageUnpacker::ElementCls UdpImageUnpacker::pop()
{
	return queues.Pop();
}


void UdpImageUnpacker::Stop()
{
	bStop = true;
}

UdpImageUnpacker::UdpImageUnpacker()
{
	bStop = false;
    width = 0;
    height = 0;
}



