///////////////////////////////////////////////////
//  AudioObject.cpp - Sourcefile for:            //
//  Interfaces: IAudioReciever, IAudioEmitter    //
//	Connectables: AudioReciever, AudioEmitter    //
//  by: Kalle Münster                            //
///////////////////////////////////////////////////
#include "AudioObject.h"


///////////////////////////////////////////////////
// Interface: IAudioReciever

//static IAudioReciever* MasterReciever;
IAudioReciever* 
AudioReciever::MasterReciever;

static bool _IsMasterStable = true;
static bool _FFTwindowGettable = false; 

IAudioReciever::IAudioReciever(void)
{
	this->IsMuted=false;
}

IAudioReciever::~IAudioReciever(void)
{}

void
IAudioReciever::InitiateListener(Transform* myTransform)
{
	IAudioReciever::MasterReciever = this;
	this->SetAudioResieverPosition(myTransform);
}

bool
IAudioReciever::SetAsMasterReciever(IAudioReciever* reciever)
{
	if(reciever==AudioReciever::MasterReciever)
	{return _IsMasterStable = true;}

	AudioReciever::MasterReciever = reciever;
	_FFTwindowGettable=false;
	return _IsMasterStable = false;
}

bool
IAudioReciever::IsMasterReciever(void)
{
	return  ((this == MasterReciever)&& _IsMasterStable);
}

bool
IAudioReciever::IsShared(bool setter)
{
	if(setter)
		return MasterReciever==this;

	return false;
}

IAudioReciever*
IAudioReciever::GetMasterReciever(void)
{
	return MasterReciever;
}

void
IAudioReciever::SetAudioResieverPosition(Transform* myTranform)
{
	if(IsMasterReciever())
	{
			BASS_Set3DPosition(&(BASS_3DVECTOR)myTranform->position, &(BASS_3DVECTOR)myTranform->movement,&(BASS_3DVECTOR)myTranform->forward,&(BASS_3DVECTOR)myTranform->up);
			AUDIO->Update3Dchanges();
			//BASS_Apply3D();
			_FFTwindowGettable=true;
	}
}

bool
IAudioReciever::ToggleMute(void)
{
	if(!IsMuted)
	{
		mutedVolume=BASS_GetVolume();
		BASS_SetVolume(0);
		this->IsMuted=true;
	}
	else
	{
		BASS_SetVolume(mutedVolume);
		IsMuted=false;
	}
	return IsMuted;
}

float
IAudioReciever::AudioVolume(float setter)
{
	if(setter<=1)
		BASS_SetVolume(setter);

	return BASS_GetVolume();
}

void
IAudioReciever::DebugOutPosition(void)
{
	BASS_3DVECTOR temp1 = BASS_3DVECTOR(0,0,0);
	BASS_3DVECTOR temp2 = BASS_3DVECTOR(0,0,0);
	BASS_3DVECTOR temp3 = BASS_3DVECTOR(0,0,0);
	BASS_3DVECTOR temp4 = BASS_3DVECTOR(0,0,0);
	BASS_Get3DPosition(&temp1,&temp2,&temp3,&temp4);
	AUDIO->Update3Dchanges();
//	BASS_Apply3D();
}

void* 
IAudioReciever::GetMasterOutFFT(void)
{
	if(AUDIO->MasterResampling() && _FFTwindowGettable)
		return AUDIO->GetMasterOutFFT();
	else 
		return &_FFTwindowGettable;
}



///////////////////////////////////////////////////
// Interface:  IAudioEmitter 

IAudioEmitter::IAudioEmitter(void)
{
	IsPlaying=false;
	for (int i=0;i<128;i++)
		fftData.buffer[i]=0;
}

void
IAudioEmitter::InitiateAudioEmitter(Transform* myTransform,const char *audioFileName)
{
	this->LoadeSample(audioFileName);
	SetMyPosition(myTransform);
	AUDIO->Update3Dchanges();
//	BASS_Apply3D();
}

IAudioEmitter::~IAudioEmitter(void)
{}

void
IAudioEmitter::AudioPause(void)
{
	if(IsPlaying)
	{
		BASS_ChannelPause(audioSource);
		IsPlaying=false;
	}
}

void
IAudioEmitter::PlayAudio(bool restart)
{
	if(!IsPlaying)
	{
		BASS_ChannelPlay(audioSource,restart);
		IsPlaying=true;
	}
}

float
IAudioEmitter::AudioVolume(float setter)
{
	if(setter<=1)
		BASS_ChannelSetAttribute(audioSource,BASS_ATTRIB_VOL,setter);

	return (float)BASS_ChannelGetAttribute(audioSource,BASS_ATTRIB_VOL,&setter);
}

float
IAudioEmitter::PitchAudio(float pitcher)
{
	if(pitcher<=1)
		
		//todo..

	throw "mach ich später...";
}

bool
IAudioEmitter::IsAudioPlaying(void)
{
	if( BASS_ChannelIsActive(audioSource) == BASS_ACTIVE_PLAYING)
		return true;
	else 
		return false;
}

void
IAudioEmitter::SetMyPosition(Transform *myTransform)
{
	BASS_ChannelSet3DPosition((DWORD)audioSource,&(BASS_3DVECTOR)myTransform->position,&(BASS_3DVECTOR)myTransform->rotation,&(BASS_3DVECTOR)myTransform->movement);
	AUDIO->Update3Dchanges();
//	BASS_Apply3D();
}

float*
IAudioEmitter::GetFFTWindow(void)
{
	return this->GetFFTData();
}

float*
IAudioEmitter::GetFFTData(void)
{
	void* buffer = &fftData.buffer[0];
	if(AUDIO->GetChannelFFT(this->audioSource,buffer,FFT_SIZE::Small))
		return &fftData.buffer[0];
	else return false;
}

void
IAudioEmitter::LoadeStream(const char* audioFileName)
{
	audioSource = AUDIO->LoadeMusic(audioFileName,LOAD_3D,(void*)this);
	this->AudioVolume(1);
}

void
IAudioEmitter::LoadeSample(const char* audioFileName,bool loop)
{
	audioSource = AUDIO->Loade3DSample(audioFileName,(void*)this,loop);
	this->AudioVolume(1);
}

void 
IAudioEmitter::Set3Dparameter(float minDistance,float maxDistance)
{
	BASS_ChannelSet3DAttributes(audioSource,BASS_3DMODE_NORMAL,minDistance,maxDistance,-1,-1,-1);
	AUDIO->Update3Dchanges();
}


///////////////////////////////////////////////////////////////
// ConnectableComponent: AudioEmitter

AudioEmitter::AudioEmitter(void)
{
	 TypeHashCode = (unsigned)typeid(AudioEmitter).hash_code();
}


void
AudioEmitter::LoadeSample(const char* audioFileName,bool loop)
{
	audioSource = AUDIO->Loade3DSample(audioFileName,(void*)this,loop);
//	SetMyPosition(&this->Connection()->transform);
	this->AudioVolume(1);
}

void
AudioEmitter::PlaySample(HCHANNEL sample)
{
	BASS_ChannelPlay(sample,true);
//	BASS_ChannelSet3DPosition(sample,&(BASS_3DVECTOR)myTransform->position,&(BASS_3DVECTOR)myTransform->rotation,&(BASS_3DVECTOR)myTransform->movement);
	AUDIO->Update3Dchanges();
}



void
AudioEmitter::LoadeStream(const char* audioFileName)
{
	audioSource = AUDIO->LoadeMusic(audioFileName,LOAD_3D,this);
//	SetMyPosition(&this->Connection()->transform);
	this->AudioVolume(1);
}



/////////////////////////////////////////////////////////////
// ConnectableComponente: AudioReciever

void
AudioReciever::DoEarly(void)
{
	if(TryGetReciever())
	{
		//Set The Position of the attached AudioReciever
		SetAudioResieverPosition(this->Connection()->getTransform());
	}
}

void
AudioReciever::DoUpdate(void)
{

}

bool 
AudioReciever::TryGetReciever(void)
{
	return SetAsMasterReciever(this);
}


