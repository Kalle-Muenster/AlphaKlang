#include "AudioObject.h"


//static IAudioReciever* MasterReciever;
IAudioReciever* 
AudioReciever::MasterReciever;

static bool _IsMasterStable = true;
static bool _FFTwindowGettable = false; 

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


IAudioReciever::IAudioReciever(void)
{
	this->IsMuted=false;
}
IAudioReciever::~IAudioReciever(void)
{

}
void
IAudioReciever::InitiateListener(Transform* myTransform)
{
	IAudioReciever::MasterReciever = this;
	this->SetAudioResieverPosition(myTransform);

	
	
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

void
IAudioReciever::SetAudioResieverPosition(Transform* myTranform)
{
//	BASS_Set3DPosition(myTranform->position.asBassVector(), &myTranform->movement,myTranform->forward.asBassVector(),myTranform->up.asBassVector());
	if(IsMasterReciever())
	{
			BASS_Set3DPosition(&(BASS_3DVECTOR)myTranform->position, &(BASS_3DVECTOR)myTranform->movement,&(BASS_3DVECTOR)myTranform->forward,&(BASS_3DVECTOR)myTranform->up);
			BASS_Apply3D();
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
	BASS_Apply3D();
//	printf("\nposition: %f,%f,%f",temp1.x,temp1.y,temp1.z);
}

void* 
IAudioReciever::GetMasterOutFFT(void)
{
	if(AUDIO->MasterResampling() && _FFTwindowGettable)
		return AUDIO->GetMasterOutFFT();
	else 
		return &_FFTwindowGettable;
}





IAudioEmitter::IAudioEmitter(void)
{
	IsPlaying=false;
	for (int i=0;i<1024;i++)
		fftwindow[i]=0;
}

void
IAudioEmitter::InitiateAudioEmitter(Transform* myTransform,const char *audioFileName)
{
	this->LoadeSample(audioFileName);
	SetMyPosition(myTransform);
	BASS_Apply3D();
}


IAudioEmitter::~IAudioEmitter(void)
{

}

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

	throw "mach ich sp�ter...";
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
	BASS_Apply3D();
}

float*
IAudioEmitter::GetFFTWindow(void)
{
	return this->GetFFTWindow(Small);
}

float*
IAudioEmitter::GetFFTWindow(int size)
{
	void* buffer = &fftwindow[0];
	if(AUDIO->GetChannelFFT(this->audioSource,buffer,(FFT_SIZE)size))
		return &fftwindow[0];
	else return false;
}


AudioEmitter::AudioEmitter(void)
{

}


void
IAudioEmitter::LoadeSample(const char* audioFileName,bool loop)
{
	
	audioSource = AUDIO->Loade3DSample(audioFileName,loop);
//	SetMyPosition(&this->Connection()->transform);
	this->AudioVolume(1);

}

void
AudioEmitter::LoadeSample(const char* audioFileName,bool loop)
{
	
	audioSource = AUDIO->Loade3DSample(audioFileName,loop);

	this->AudioVolume(1);

}

void
AudioEmitter::PlaySample(HCHANNEL sample,bool loop)
{
	audioSource = sample;
	BASS_ChannelPlay(sample,false);
}

void
IAudioEmitter::LoadeStream(const char* audioFileName)
{
	audioSource = AUDIO->LoadeMusic(audioFileName,LOAD_3D);
//	SetMyPosition(&this->Connection()->transform);
	this->AudioVolume(1);
}

void
AudioEmitter::LoadeStream(const char* audioFileName)
{
	audioSource = AUDIO->LoadeMusic(audioFileName,LOAD_3D);
	SetMyPosition(this->Connection()->getTransform());
	this->AudioVolume(1);
}





