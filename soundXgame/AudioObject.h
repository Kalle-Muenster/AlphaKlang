#ifndef _AUDIO_OBJECT_H_
#define _AUDIO_OBJECT_H_


#include <bass.h>
#include <bass_fx.h>

#include "projectMacros.h"
#include "DataStructs.h"
#include "UpdateManager.h"
#include "Connectable.h"


template<const unsigned short fftBufferSize> struct FFT_DATA_BUFFER
{
 float buffer[fftBufferSize];
};

/* Interface, not instanziable */

class IAudioEmitter
{
protected:
	
	void InitiateAudioEmitter(Transform*,const char*);
	bool IsPlaying;
	void SetMyPosition(Transform*);
	FFT_DATA_BUFFER<128> fftData;

public:
	HCHANNEL audioSource;
	IAudioEmitter(void);
	virtual ~IAudioEmitter(void)=0;
	virtual void LoadeSample(const char*,bool loop = true);
	virtual void LoadeStream(const char*);
	virtual void PlayAudio(bool restart = true);
	virtual void AudioPause(void);
	virtual float AudioVolume(float=2);
	virtual float PitchAudio(float=2);
	virtual float* GetFFTData(void);
	virtual float* GetFFTWindow(void);
	virtual bool IsAudioPlaying(void);
	virtual void Set3Dparameter(float minDistance,float maxDistance);
};

/* Connetable of IAudioEmitter */
class AudioEmitter : public IConnectable , public IAudioEmitter, public IUpdateble
{
public:
	virtual void PlaySample(HCHANNEL sample);
	AudioEmitter(void);
	virtual void LoadeSample(const char*,bool loop = true);
	virtual void LoadeStream(const char*);
	virtual void DoUpdate(void)
	{
		if(this->IsAudioPlaying())
			SetMyPosition(this->Connection()->getTransform());
	}
		
};




/* Interface, not instanziable */
class IAudioReciever 
{	
protected:
	static IAudioReciever* MasterReciever;
	float mutedVolume;
	bool IsMuted;
	void SetAudioResieverPosition(Transform*);
	virtual bool IsShared(bool=NULL);
	virtual IAudioReciever* GetMasterReciever(void); 

	

public:
	IAudioReciever(void);
	virtual ~IAudioReciever(void);
	virtual void InitiateListener(Transform*);
	virtual bool ToggleMute(void);
	virtual float AudioVolume(float=2);
	void DebugOutPosition(void);
	void* GetMasterOutFFT(void);
	static bool SetAsMasterReciever(IAudioReciever*);
	bool IsMasterReciever();
};

/* Connectable of AudioReciever */
class AudioReciever : 
	public IConnectable, 
	public IAudioReciever, 
	public IUpdateble
{
public:
		virtual void DoEarly(void);
		virtual void DoUpdate(void);
		bool TryGetReciever(void);



};
#endif