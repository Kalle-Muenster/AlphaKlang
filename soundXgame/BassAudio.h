#ifndef _BASS_AUDIO_H_INCLUDET
#define _BASS_AUIDO_H_INCLUDET

#include <bass.h>
//#include <baseaudioprocessingobject.h>
#include <tags.h>
#include <BASS_DSHOW.h>
#include <bass_fx.h>
#include <bassmix.h>
#include "DataStructs.h"

#define FFT_WINDOW_SIZE (1024)

	enum FFT_SIZE
	{Small=256,Medium=512,Large=1024};

enum GET_DATA_MODE
	{FFT=0,BYTES=1,FLOATS=2};

enum LOAD_MODE
	{LOAD_2D=0,LOAD_3D=0x1,LOAD_MONO=0x2,LOAD_STREAM=0x4,LOAD_SAMPLE=0x8};

class BassAudio
{

public:
	
	virtual ~BassAudio(void);
	static BassAudio* GetInstance(void);


	HSTREAM LoadeMusic(const char*,LOAD_MODE);
	HCHANNEL Loade3DSample(const char*);
	void LoadeBackgroundAudio(const char*);// or musik...
	void Play(void);
	void Pause(void);
	void SetListenerPosition(Transform*);
	float Volume(float=2);
	void* GetChannelBuffer(DWORD channel,int sizeInBytes);
	unsigned GetChannelFFT(DWORD channel,void* buffer);
	unsigned GetChannelFFT(DWORD channel,void* buffer, FFT_SIZE);
	void* GetBackgroundAudioFFT(void);
	void* GetBackgroundAudioFFT(FFT_SIZE);
	bool ToggleMasterResampling(void);
	virtual bool MasterResampling(BOOL = 3);
	void* GetMasterOutFFT(void);
	void PerFrameReset(void);

private:

	BassAudio(void);
};

#endif