#ifndef _BASS_AUDIO_H_INCLUDET
#define _BASS_AUIDO_H_INCLUDET

// AUDIO->
/*
	Everything Audio...
	use to configures I/O drivers,configure FX-rooting's,
	Sampling,Loading,Streaming Files or other sources... 
	Configuring the 3D-Environment-settings,
	Playing Background-Musik and controling the master-output.
	also Provides Getting FFT-Data for any audio chanel in Scene..
*/
//#ifndef _POJECT_MACROS__
#define AUDIO BassAudio::GetInstance()
//#endif

#define AUDIO_BUFFERS_UPDATE_PERIOD			(100)
#define FFT_WINDOW_SIZE						(1024)
#define MAXIMUM_NUMBER_OF_AUDIOOBJECTS		(1024)

#include <vector>
#include <bass.h>
#include <tags.h>
//#include <BASS_DSHOW.h>
#include <bass_fx.h>
#include <bassmix.h>
#include "DataStructs.h"
#include "Connectable.h"
#include "Utility.h"




enum FFT_SIZE
	{Small=256,Medium=512,Large=1024};

enum GET_DATA_MODE
	{FFT=0,BYTES=1,FLOATS=2};

enum LOAD_MODE
	{LOAD_2D=0,LOAD_3D=0x1,LOAD_MONO=0x2,LOAD_STREAM=0x4,LOAD_SAMPLE=0x8,FX_CHANNEL=0xF};

class BassAudio
{

public:
	
	virtual ~BassAudio(void);
	HCHANNEL GetSampleFromBank(unsigned);
	HCHANNEL LoadeSampleToBank(unsigned &,const char* fileNaname);
	static BassAudio* GetInstance(void);
	static const char* start(void);
	HSTREAM LoadeMusic(const char*,LOAD_MODE,void* loadingObject);
	HCHANNEL Loade3DSample(const char*,void* loadingObject,bool loop = true);
	void LoadeBackgroundAudio(const char*);// or musik...
	void Play(void);
	void Pause(void);
	void SetListenerPosition(Transform*);
	float Volume(float=2.f);
	float BackgroundMusicVolume(float=2.f);
	void* GetChannelBuffer(DWORD channel,int sizeInBytes);
	unsigned GetChannelFFT(DWORD channel,void* buffer, FFT_SIZE=FFT_SIZE::Small);
	void* GetBackgroundAudioFFT(FFT_SIZE=FFT_SIZE::Small);
	bool ToggleMasterResampling(void);
	virtual bool MasterResampling(BOOL = 3);
	void* GetMasterOutFFT(void);
	void PerFrameReset(void);
	float Set3D_DistanceFactor(float distanceFactor);
	float Set3D_RollOffFactor(float rollOffFactor);
	float Set3D_DopplerFXFactor(float dopplerFxFactor);
	void Update3Dchanges(void);
	IObjection<IConnectable>* FindObjectByAudiochannel(DWORD channel);	//better outsource to SCENE!...
	HCHANNEL GetBackgroundChannelHandle(void);

private:
	float distance;
	float rollOff;
	float doppler;
	std::vector<HCHANNEL> SampleBank;
	BassAudio(void);
	

};

#endif