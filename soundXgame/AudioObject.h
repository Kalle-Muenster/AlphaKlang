#ifndef _AUDIO_OBJECT_H_
#define _AUDIO_OBJECT_H_


#include "Utility.h"



class IAudioEmitter
{
protected:
	HCHANNEL audioSource;
	void InitiateAudioEmitter(TransformA*,const char*);
	bool IsPlaying;
	void SetMyPosition(TransformA*);
	float fftwindow[1024];
public:
	IAudioEmitter(void);
//	IAudioEmitter(Transform*);
	virtual ~IAudioEmitter(void)=0;
	virtual void LoadeSample(const char*);
	virtual void LoadeStream(const char*);
	virtual void PlayAudio(void);
	virtual void AudioPause(void);
	virtual float AudioVolume(float=2);
	virtual float PitchAudio(float=2);
	virtual float* GetFFTWindow(void);
	virtual float* GetFFTWindow(int);
	virtual bool IsAudioPlaying(void);

};

class AudioEmitter : public IConnectable , public IAudioEmitter, public IUpdateble
{

public:
	AudioEmitter(void);
	virtual void LoadeSample(const char*);
	virtual void LoadeStream(const char*);
	virtual void DoUpdate(void)
	{
		if(this->IsAudioPlaying())
			SetMyPosition(this->Connection()->getTransform());
	}
		
};


class IAudioReciever 
{
protected:
	float mutedVolume;
	bool IsMuted;
	void SetMyPosition(TransformA*);
	virtual volatile bool IsShared(bool=NULL);
	virtual volatile IAudioReciever* GetMasterReciever(void); 

	

public:
	IAudioReciever(void);
	virtual ~IAudioReciever(void);
	virtual void InitiateListener(TransformA*);
	virtual bool ToggleMute(void);
	virtual float AudioVolume(float=2);
	void DebugOutPosition(void);
	void* GetMasterOutFFT(void);
	static bool SetAsMasterReciever(IAudioReciever*);
	bool IsMasterReciever();
};

class AudioReciever : 
	public IConnectable, 
	public IAudioReciever, 
	public IUpdateble
{
public:
		virtual void DoEarly(void);
		virtual void DoUpdate(void)
		{
		
		}
		bool TryGetReciever(void);

};
#endif