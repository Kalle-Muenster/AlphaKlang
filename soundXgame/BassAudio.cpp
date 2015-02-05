#include "BassAudio.h"
#include <iostream>
#include <vector>

#define DEBUG

#ifdef DEBUG


string debugStrings[50];
#endif

void initDebug(void)
{
#ifdef DEBUG
debugStrings[0]	= "all is OK									\n";
debugStrings[1]	= "memory error									\n";
debugStrings[2]	= "can't open the file							\n";
debugStrings[3]	= "can't find a free/valid driver				\n";
debugStrings[4]	= "the sample buffer was lost					\n";
debugStrings[5]	= "invalid handle								\n";
debugStrings[6]	= "unsupported sample format					\n";
debugStrings[7]	= "invalid position								\n";
debugStrings[8]	= "BASS_Init has not been successfully called	\n";
debugStrings[9]	= "BASS_Start has not been successfully called	\n";
debugStrings[14] = "already initialized/paused/whatever				\n" ;
debugStrings[18] = "can't get a free channel						\n" ;
debugStrings[19] = "an illegal type was specified					\n" ;
debugStrings[20] = "an illegal parameter was specified				\n" ;
debugStrings[21] = "no 3D support									\n" ;
debugStrings[22] = "no EAX support									\n" ;
debugStrings[23] = "illegal device number							\n" ;
debugStrings[24] = "not playing										\n" ;
debugStrings[25] = "illegal sample rate								\n" ;
debugStrings[27] = "the stream is not a file stream					\n" ;
debugStrings[29] = "no hardware voices available					\n" ;
debugStrings[31] = "the MOD music has no sequence data				\n" ;
debugStrings[32] = "no internet connection could be opened			\n" ;
debugStrings[33] = "couldn't create the file						\n" ;
debugStrings[34] = "effects are not available						\n" ;
debugStrings[37] = "requested data is not available					\n" ;
debugStrings[38] = "the channel is a \"decoding channel\"			\n" ;
debugStrings[39] = "a sufficient DirectX version is not installed	\n" ;
debugStrings[40] = "connection timedout								\n" ;
debugStrings[41] = "unsupported file format							\n" ;
debugStrings[42] = "unavailable speaker								\n" ;
debugStrings[43] = "invalid BASS version (used by add-ons)			\n" ;
debugStrings[44] = "codec is not available/supported				\n" ;
debugStrings[45] = "the channel/file has ended						\n" ;
debugStrings[46] = "the device is busy								\n" ;
debugStrings[49] = "some other mystery problem						\n" ;

#endif
}


char* _GetErrorString()
{
#ifdef DEBUG
	int error = BASS_ErrorGetCode();
	if(error == -1)
		error = 49;
	//if(error==BASS_ERROR_FX_NODECODE)
	//	return "Not a decoding channel. ";
	//if(error ==BASS_ERROR_FX_BPMINUSE)
	//	return "BPM / Beat detection is in use. ";


	return debugStrings[error];
#endif
return "";
}

char* _GetInputDeviceTypeString(DWORD result)
{
	switch (result)
	{																	 
case BASS_INPUT_TYPE_DIGITAL: return "Digital input source, for example, a DAT or audio CD.";
case BASS_INPUT_TYPE_LINE	: return "Line-in. On some devices, \"Line-in\" may be combined with other analog sources into a single BASS_INPUT_TYPE_ANALOG input. ";
case BASS_INPUT_TYPE_MIC	: return " Microphone.  ";						
case BASS_INPUT_TYPE_SYNTH	: return "Internal MIDI synthesizer. ";
case BASS_INPUT_TYPE_CD		: return "Analog audio CD."; 
case BASS_INPUT_TYPE_PHONE	: return "Telephone."; 
case BASS_INPUT_TYPE_SPEAKER: return " PC speaker."; 
case BASS_INPUT_TYPE_WAVE	: return "The device's WAVE/PCM output.  ";
case BASS_INPUT_TYPE_AUX	: return "Auxiliary. Like \"Line-in\", \"Aux\" may be combined with other analog sources into a single BASS_INPUT_TYPE_ANALOG input on some devices."; 
case BASS_INPUT_TYPE_ANALOG	: return "Analog, typically a mix of all analog sources.";
case BASS_INPUT_TYPE_UNDEF	: return "? Undefined Input Device..."; 
default :
	return _GetErrorString(); 
}
}

struct Channel
{
	DWORD ID;
	char Name[64];
};

float _BackgroundFFTbuffer[FFT_WINDOW_SIZE];
int _BackgroundFFTcurrentSize = FFT_WINDOW_SIZE;
HSTREAM derAudio;
HRECORD MasterOutResample;
List<void*,MAXIMUM_NUMBER_OF_AUDIOOBJECTS> _GameObjects;
Channel _Channels[MAXIMUM_NUMBER_OF_AUDIOOBJECTS];



bool _BackgroundAudioIsPlaying = false;
bool _AudioNeed3DUpdate = false;
bool _BackroundFFTcalculated = false;
bool _IsPlaying = false;
bool _IsRecordingMaster = false;
bool _backgroundAudioLoadet = false;
bool _recordingInitiated = false;


BassAudio::~BassAudio(void)
{

}

int _GetNumberOfInputDevices(void)
{
	BASS_RECORDINFO *info = new BASS_RECORDINFO();
	if(!BASS_RecordGetInfo(info))
	{
		std::cout<<"AUDIO: Error-> ";
		std::cout<<_GetErrorString();
		std::cout<<"\n";
		return -1;
	}
	else
		return info->inputs;
}

void _printInputDevices(void)
{
	int Inputs = _GetNumberOfInputDevices();
	for (--Inputs;Inputs > -1;Inputs--)
		printf("AUDIO: InputDevice %i: %s\n",Inputs,_GetInputDeviceTypeString(BASS_RecordGetInput(Inputs,NULL)));
}




void
_get3Dfactors(float& dist,float& rollOff,float& dopplerF)
{
	 if(BASS_Get3DFactors(&dist,&rollOff,&dopplerF))
	{
		std::cout<<"AUDIO: 3D-settings:\n";
		std::cout<<"         Distance-factor: ";
		printf("%f\n",dist);
		std::cout<<"          RollOff-factor: ";
		printf("%f\n",rollOff);
		std::cout<<"          Doppler-factor: ";
		printf("%f\n\n",dopplerF);
	}
}

bool
_set3Dfactors(float distance,float rollOff,float doppler)
{
	 bool result = BASS_Set3DFactors(distance,rollOff,doppler);

	 if(!result)
		printf("AUDIO: %s\n",_GetErrorString());
	 else
		_AudioNeed3DUpdate=true;

	 return result;
}

int 
_getMasterOutForRecord(void)
{
	_printInputDevices();
	DWORD flags;
	for (int i=0; (flags=BASS_RecordGetInput(i, NULL))!=-1; i++) 
	{
		if ((flags&BASS_INPUT_TYPE_MASK)==BASS_INPUT_TYPE_WAVE) 	
		{ // found the waveout!
			return i;
		}
	}
	printf("AUDIO: Try open InputDevice:0 for Recording...\n");
	return 0;
}

bool 
_ToggleRecordFromMasterOut(void)
{
	if(!_recordingInitiated)
	{
		BASS_RecordInit(-1);
		printf("AUDIO: %s\n",_GetErrorString());
		BASS_RecordSetInput(_getMasterOutForRecord(), BASS_INPUT_ON, 1);
		printf("AUDIO: %s\n",_GetErrorString());
		MasterOutResample = BASS_RecordStart(44100,2,BASS_RECORD_PAUSE,NULL,NULL);
		printf("AUDIO: %s\n",_GetErrorString());
		_recordingInitiated=true;
	}

	if(_IsRecordingMaster)
	{
			BASS_ChannelPause(MasterOutResample);
			_IsRecordingMaster=false;
	}
	else
	{
		BASS_ChannelPlay(MasterOutResample,false);
		_IsRecordingMaster=true;
	}
	return _IsRecordingMaster;
}



HSTREAM
_getAudioStreamByFileName(const string filename,int mode)
{
	/*
	FILE* file;
	file = fopen(filename,"rb");
	long fileLength;
	fseek(file,0,SEEK_END);
	fileLength=ftell(file);
	fseek(file,0,SEEK_SET);
	fclose(file);  	 */

	FILE* file = fopen(filename,"rb");
	long fileLength=fseek(file,0,SEEK_END);
	fclose(file);	 

	bool _FX=mode>=0xF?true:false;
	mode %= 0xF; 

	DWORD creationFlags = NULL;

	if(mode==LOAD_3D)
	{
		_AudioNeed3DUpdate=true;
		creationFlags = _FX ? BASS_STREAM_AUTOFREE|BASS_SAMPLE_LOOP|BASS_SAMPLE_MONO|BASS_SAMPLE_3D|BASS_SAMPLE_FX 
							: BASS_STREAM_AUTOFREE|BASS_SAMPLE_LOOP|BASS_SAMPLE_MONO|BASS_SAMPLE_3D;
	}
	else if(mode==LOAD_2D)
		creationFlags = _FX ? BASS_STREAM_AUTOFREE|BASS_SAMPLE_LOOP|BASS_SAMPLE_FX
							: BASS_STREAM_AUTOFREE|BASS_SAMPLE_LOOP;
	
	return BASS_StreamCreateFile(false, filename,0,fileLength,creationFlags);
}

HSAMPLE
_getSampleHandleByFilename(const char* tmp,bool loop)
{
	char* filename = new char[strlen(tmp)+1+5]();
	sprintf(&filename[0],"Data/%s",tmp);

	FILE* file;
	file = fopen(filename,"rb");
	fseek(file,0,SEEK_END);
	long fileLength=ftell(file);
	fseek(file,0,SEEK_SET);
	fclose(file);
	long offset=0;
	DWORD loadingFlags;

	if(loop)
		loadingFlags = BASS_SAMPLE_MONO|BASS_SAMPLE_LOOP|BASS_SAMPLE_3D;
	else
		loadingFlags = BASS_SAMPLE_MONO|BASS_SAMPLE_3D;

	return BASS_SampleLoad(false,filename,offset,0,20,loadingFlags);
}

DWORD
_getChannelData(DWORD channel,int mode,int size,void* buffer)
{
	DWORD flags = (mode==FFT) ? (size== 256) ? BASS_DATA_FFT256 
							  : (size== 512) ? BASS_DATA_FFT512 
							  : (size==1024) ? BASS_DATA_FFT1024 
								             : size 
				: (mode==BYTES) ? size 
								: BASS_DATA_FLOAT;

	return BASS_ChannelGetData(channel,buffer,flags);
}

HFX
_setChannelFX(DWORD channel,unsigned BASSfxName,int priority=0)
{
	HFX fx = BASS_ChannelSetFX(channel,BASSfxName,priority);
	if(fx==NULL)
		printf("AUDIO: ERROR: %s \n",_GetErrorString());
	return fx;
}

void
_removeChannelFX(DWORD channel,HFX fx)
{
	BASS_ChannelRemoveFX(channel,fx);
}

void
_setFXparameter(HFX fx,void* parameterStruct)
{
	BASS_FXSetParameters(fx,parameterStruct);
}

void
_LinkChannelControll(DWORD channelA,DWORD channelB)
{
	BASS_ChannelSetLink(channelA,channelB);

}

HCHANNEL
BassAudio::Loade3DSample(const char* filename,void* loadingObject,bool loop)
{
	HSAMPLE	sample = _getSampleHandleByFilename(filename,loop);
	
	if(sample!=NULL)
	{
		HCHANNEL channel = BASS_SampleGetChannel(sample,true);
		if(channel!=NULL)
		{
			Channel chan;
			chan.ID = channel;
			int i = -1;
			while((++i<64) && ((chan.Name[i] = filename[i])!='\0'));
			i = _GameObjects.Add(loadingObject);
			if(_Channels[i].ID!=NULL)
				BASS_ChannelStop(_Channels[i].ID);
			_Channels[i]=chan;
			_AudioNeed3DUpdate=true;
			return channel;
		}
	}

	std::cout<<_GetErrorString();
	return NULL;
}



void*
BassAudio::GetChannelBuffer(DWORD channel,int sizeInByte)
{
	void* buffer;
	_getChannelData(channel,(int)BYTES,sizeInByte,buffer);
	return buffer;
}


unsigned
BassAudio::GetChannelFFT(DWORD channel,void* buffer,FFT_SIZE size)
{
	return _getChannelData(channel,(int)FFT,(int)size,buffer);
}

void*
BassAudio::GetBackgroundAudioFFT(FFT_SIZE size)
{
	void* buffer = &_BackgroundFFTbuffer[0];
	
	if((_IsPlaying)&&((!_BackroundFFTcalculated) || (_BackgroundFFTcurrentSize!=size)))
	{
		if(GetChannelFFT(derAudio,buffer,size)==0)
			printf("AUDIO: %s\n",_GetErrorString());

		_BackgroundFFTcurrentSize = size;
		_BackroundFFTcalculated = true;
	}
	return buffer;
}

void
BassAudio::PerFrameReset(void)
{
	_BackroundFFTcalculated = false;

	if(_AudioNeed3DUpdate)
	{	BASS_Apply3D();
		_AudioNeed3DUpdate=false; }
}



HCHANNEL 
BassAudio::GetSampleFromBank(unsigned slotNumber)
{
	return SampleBank[slotNumber];
}

HCHANNEL 
BassAudio::LoadeSampleToBank(unsigned& slotNumber,const char* filename)
{
	SampleBank.push_back(BASS_SampleGetChannel(_getSampleHandleByFilename(filename,true),true));
	slotNumber = SampleBank.size()-1;
	return SampleBank[slotNumber];
}

BassAudio::BassAudio(void)
{
	distance=rollOff=doppler=0;
	SampleBank = std::vector<HCHANNEL>();
	initDebug();
	BASS_Init(-1,44100,BASS_DEVICE_3D,GetActiveWindow(),NULL);
	printf("Loading BASS-AUDIO: %s\n", _GetErrorString());
	BASS_INFO *info = new BASS_INFO();
	if( BASS_GetInfo(info))
		printf("Detected DirectSound version: %i\n",info->dsver);
	BASS_SetConfig(BASS_CONFIG_3DALGORITHM,BASS_3DALG_DEFAULT);
	//std::cout<<_GetErrorString();
	//HPLUGIN plgnBASSFX = BASS_PluginLoad("bass_fx.dll",BASS_UNICODE);
	//std::cout<<_GetErrorString();
	//HPLUGIN plgnBASSMIX = BASS_PluginLoad("bassmix.dll",BASS_UNICODE);
	//std::cout<<_GetErrorString();
	//HPLUGIN plgnBASSTAG = BASS_PluginLoad("tags.dll",BASS_UNICODE);
	//std::cout<<_GetErrorString();
	BASS_SetConfig(BASS_CONFIG_UPDATEPERIOD, AUDIO_BUFFERS_UPDATE_PERIOD);
	printf("Set BufferUpdatePeriod to: %i\n",AUDIO_BUFFERS_UPDATE_PERIOD);
	BASS_SetConfig(BASS_CONFIG_REC_BUFFER,1500);
	char stringBuffer[32];
	_itoa(BASS_Mixer_GetVersion(),&stringBuffer[0],16);
	printf("Bass-Mixer-version: %c.%c.%c.%c\n",stringBuffer[0],stringBuffer[2],stringBuffer[4],stringBuffer[6]);
	_itoa(BASS_FX_GetVersion(),&stringBuffer[0],16);
	printf("Bass-FX-version: %c.%c.%c.%c\n",stringBuffer[0],stringBuffer[2],stringBuffer[4],stringBuffer[6]);
	_itoa(TAGS_GetVersion(),&stringBuffer[0],16);
	printf("Bass-Tags-version: %c.%c.%c.%c\n",stringBuffer[0],stringBuffer[2],stringBuffer[4],stringBuffer[6]);
	_get3Dfactors(distance,rollOff,doppler);

	for(Channel channel : _Channels)
	{
		channel.ID=NULL;
	}

}



float
BassAudio::Set3D_DistanceFactor(float distanceFactor)
{
	 this->distance = distanceFactor;
	 if(!_set3Dfactors(this->distance,this->rollOff,this->doppler))
		  _get3Dfactors(this->distance,this->rollOff,this->doppler); 
	 else
		 printf("AUDIO: 3D-DistanceFactor set to: %f \n",this->distance);
	 return this->distance;
}

float
BassAudio::Set3D_RollOffFactor(float rollOffFactor)
{
	 this->rollOff = rollOffFactor;
	 if(!_set3Dfactors(this->distance,this->rollOff,this->doppler))
		  _get3Dfactors(this->distance,this->rollOff,this->doppler);
	 else
		 printf("AUDIO: 3D-RollOff-Factor set to: %f \n",this->rollOff);
	 return this->rollOff;
}

float
BassAudio::Set3D_DopplerFXFactor(float dopplerFxFactor)
{
	this->doppler = dopplerFxFactor;
	if(!_set3Dfactors(this->distance,this->rollOff,this->doppler))
		  _get3Dfactors(this->distance,this->rollOff,this->doppler);
	else
		 printf("AUDIO: 3D-DopplerFactor set to: %f \n",this->doppler);
	return this->doppler;
}


float 
BassAudio::BackgroundMusicVolume(float setter)
{	if(_backgroundAudioLoadet)
	{
	 	if(setter<=1)
		{
			BASS_ChannelSetAttribute(derAudio,BASS_ATTRIB_VOL,setter);
			return setter;
		}

	BASS_ChannelGetAttribute(derAudio,BASS_ATTRIB_VOL,&setter);
	return  setter;
	}
	else 
		return 0;
}

bool
BassAudio::MasterResampling(BOOL Switch)
{
	if((Switch<3)&&(Switch!=_IsRecordingMaster))
		_IsRecordingMaster = _ToggleRecordFromMasterOut();

	return _IsRecordingMaster;
}

bool
BassAudio::ToggleMasterResampling(void)
{
	 _IsRecordingMaster =  _ToggleRecordFromMasterOut();
	 return _IsRecordingMaster;
}

HSTREAM
BassAudio::LoadeMusic(const char* tmp,LOAD_MODE mode,void* loadingObject)
{
	char* filename = new char[strlen(tmp)+1+5]();
	sprintf(&filename[0],"Data/%s",tmp);

	Channel chan;
	chan.ID = _getAudioStreamByFileName((const string)filename,(int)mode);
	int i = -1;
	while((++i < 64) && ((chan.Name[i]=filename[i])!='\0'));
	i = _GameObjects.Add(loadingObject);
	if(_Channels[i].ID!=NULL)
		 BASS_ChannelStop(_Channels[i].ID);
	_Channels[i] = chan;

	delete[] filename;
	return chan.ID;
}

void
BassAudio::Update3Dchanges(void)
{
	 _AudioNeed3DUpdate=true;
}


void 
BassAudio::SetListenerPosition(Transform* cameraTranform)
{
	BASS_Set3DPosition(&(BASS_3DVECTOR)cameraTranform->position,&(BASS_3DVECTOR)cameraTranform->movement,&(BASS_3DVECTOR)cameraTranform->forward,&(BASS_3DVECTOR)cameraTranform->up);
	printf("AUDIO: %s\n",_GetErrorString());
	_AudioNeed3DUpdate=true;
}

void
BassAudio::LoadeBackgroundAudio(const char* tmp)
{
	char* filename = new char[strlen(tmp)+1+5]();
	sprintf(&filename[0],"Data/%s",tmp);

	derAudio = _getAudioStreamByFileName((string)filename,LOAD_2D);
	BASS_ChannelSetDevice(derAudio,-1);
	printf("AUDIO: %s loadet as Backgroundmusic !\n",filename);
	_backgroundAudioLoadet = true;

	delete[] filename;
}

void
BassAudio::Play(void)
{
	if(_backgroundAudioLoadet)
	{
		if(BASS_ChannelPlay(derAudio,true))
		{
			_IsPlaying=true;
			std::cout<<"AUDIO:  PLAYING\n";
		}
		else
			printf("AUDIO: %s\n",_GetErrorString());
	}
}

void 
BassAudio::Pause(void)
{
	if(_IsPlaying)
	{
		BASS_ChannelPause(derAudio);
		_IsPlaying= false;
		std::cout<<"AUDIO:  PAUSED\n";
	}
}

float
BassAudio::Volume(float vol)
{
	if (vol<=1)
		BASS_SetVolume(vol);
	vol = BASS_GetVolume();
	printf("AUDIO: set master-volume to: %f\n",vol);
	return vol; 
}

void*
BassAudio::GetMasterOutFFT(void)
{
	void* buffer;
	if(_IsRecordingMaster)
		GetChannelFFT(MasterOutResample,buffer,Small);
	printf("AUDIO: %s\n",_GetErrorString());
	return buffer;
}


IObjection<IConnectable>* 
BassAudio::FindObjectByAudiochannel(DWORD channel)
{
	int i = -1;
	while((++i<MAXIMUM_NUMBER_OF_AUDIOOBJECTS)&&(_Channels[i].ID!=channel));
	return ((IConnectable*)_GameObjects[i])->Connection();
}

HCHANNEL
BassAudio::GetBackgroundChannelHandle(void)
{
	return derAudio;
}
