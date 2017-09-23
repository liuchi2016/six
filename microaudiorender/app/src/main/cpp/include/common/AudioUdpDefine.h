#ifndef _AUDIO_UDP_DEFINE_
#define _AUDIO_UDP_DEFINE_
struct AudioUdpHeader
{
	uint32_t	sampleRate;
	uint32_t	channels;
	uint32_t	channel_layout;
	uint32_t	sampleFormat;
	uint32_t 	frame_index;
};
#endif
