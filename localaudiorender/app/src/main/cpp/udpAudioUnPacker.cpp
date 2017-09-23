//#include "StdAfx.h"
#include "udpAudioUnPacker.h"

void UdpAudioUnpacker::Flush(uint8_t* buf ,int size){
	if (size > sizeof(AudioUdpHeader)){
		AudioUdpHeader& hdr = *(AudioUdpHeader*)buf;
		int m_iSampleRate = hdr.sampleRate;
		int m_iChannels = hdr.channels;
		int m_iChannelLayout = hdr.channel_layout;
		int m_iSampleFormat = hdr.sampleFormat;
		decoder.Initialize(m_iSampleRate, m_iChannels, m_iChannelLayout, m_iSampleFormat);
		queues.Push(new BufferCls(buf + sizeof(AudioUdpHeader),size - sizeof(AudioUdpHeader)));
	}
}


UdpAudioUnpacker::ElementCls UdpAudioUnpacker::pop()
{
	return queues.Pop();
}

void UdpAudioUnpacker::Get(uint8_t*& out,int& out_len)
{
	ElementCls  element = queues.Pop();
	decoder.Run(element->get(),element->size(),out,out_len);
}

UdpAudioUnpacker::UdpAudioUnpacker(){
	decoder.SetFreq(44100);
}



