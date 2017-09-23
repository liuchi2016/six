#ifndef _AAC_DECODER_H_2017_03_27_LC_
#define _AAC_DECODER_H_2017_03_27_LC_
#include <boost/function.hpp>
#include <common/stringexcept.h>


extern "C"
{
#include <libavformat/avformat.h>
#include <libswresample/swresample.h>
};

#include "logger.h"

struct   AACDecoder
{
public:
	static const int max_audio_buffer_size = 4096;

	AACDecoder(){
		output_codec_context = 0;
		audio_convert_ctx = 0;
		bInitailized = false;
	}

	~AACDecoder(){
		UnInitialize();
	}

public:

	void    SetFreq(int freq){
		this->freq = freq;
	}

	virtual void Run(uint8_t* yuv,int length,uint8_t*& out,int& out_len)
	{
		AVFrame*frame = av_frame_alloc();
		int len = 0; 
		int got_frame = 0;
		AVPacket  packet;
		av_init_packet(&packet);
		packet.data = yuv;
		packet.size  = length;
		len = avcodec_decode_audio4(output_codec_context, frame, &got_frame, &packet);
		if(len >= 0 && got_frame) {
			swr_convert(
				audio_convert_ctx,
				&out, max_audio_buffer_size,
				(const uint8_t **)frame->data , frame->nb_samples);
			out_len = max_audio_buffer_size;
		} 
		av_free(frame);
	}

	void    UnInitialize()
	{
		if(output_codec_context){
			avcodec_close(output_codec_context);
			output_codec_context = 0;
		}
		if (audio_convert_ctx){
			swr_free(&audio_convert_ctx);
		}
		bInitailized = false;
	}

	void   Initialize(int rate,int channels,int channel_layout,int format)
	{

		if (!bInitailized){
            LOGV("aacdecoder initialing");
			av_register_all();
			AVCodec* output_codec=avcodec_find_decoder(CODEC_ID_AAC);
			if (!output_codec)
				throw  string_exception("not found aac decoder");
			output_codec_context = avcodec_alloc_context3(output_codec);
			output_codec_context->codec_type = AVMEDIA_TYPE_AUDIO;
			output_codec_context->sample_rate = rate;
			output_codec_context->channels = channels;
			output_codec_context->sample_fmt =(AVSampleFormat)format;
			output_codec_context->channel_layout= channel_layout;
			AVRational time_base={1, output_codec_context->sample_rate};
			output_codec_context->time_base = time_base;
			output_codec_context->codec_tag = 0;
			if (avcodec_open2(output_codec_context, output_codec, NULL) < 0) 
				throw  string_exception("open aac decoder failed");
			uint64_t out_channel_layout=AV_CH_LAYOUT_STEREO;
			AVSampleFormat out_sample_fmt = AV_SAMPLE_FMT_S16;
			int out_sample_rate = freq;
			audio_convert_ctx = swr_alloc();
			audio_convert_ctx = swr_alloc_set_opts(
				audio_convert_ctx,
				out_channel_layout, 
				out_sample_fmt,
				out_sample_rate,
				channel_layout,
				(AVSampleFormat)format, 
				rate,
				0,
				NULL);
			swr_init(audio_convert_ctx);
			bInitailized = true;
            LOGV("aacdecoder initialized");
		}
	}
private:
	AVCodecContext *output_codec_context;
	bool	  bInitailized;
	struct SwrContext *audio_convert_ctx;
	int       freq;
};


#endif
