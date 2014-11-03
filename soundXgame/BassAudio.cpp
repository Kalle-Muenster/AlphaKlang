#include "BassAudio.h"
#include <iostream>

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
	if(error==BASS_ERROR_FX_NODECODE)
		return "Not a decoding channel. ";
	if(error ==BASS_ERROR_FX_BPMINUSE)
		return "BPM / Beat detection is in use. ";


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

int _GetNumberOfInputDevices(void)
{
	BASS_RECORDINFO *info = new BASS_RECORDINFO();
	if(!BASS_RecordGetInfo(info))
	{
		std::cout<<"\nAUDIO: Error-> ";
		std::cout<<_GetErrorString();
		return -1;
	}
	else
		return info->inputs;
}

void _printInputDevices(void)
{
	int Inputs = _GetNumberOfInputDevices();
	for (--Inputs;Inputs > -1;Inputs--)
		printf("\nAUDIO: InputDevice %i: %s",Inputs,_GetInputDeviceTypeString(BASS_RecordGetInput(Inputs,NULL)));
}

BassAudio* bassaudioInstance;
HSTREAM derAudio;
HRECORD MasterOutResample;
bool _IsPlaying = false;
bool _IsRecordingMaster = false;
bool _backgroundAudioLoadet = false;
bool _recordingInitiated = false;

int _getMasterOutForRecord(void)
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
	printf("\nAUDIO: Try open InputDevice:0 for Recording...");
	return 0;
}

bool _ToggleRecordFromMasterOut(void)
{
	if(!_recordingInitiated)
	{
		BASS_RecordInit(-1);
		printf("\nAUDIO: %s",_GetErrorString());
		BASS_RecordSetInput(_getMasterOutForRecord(), BASS_INPUT_ON, 1);
		printf("\nAUDIO: %s",_GetErrorString());
		MasterOutResample = BASS_RecordStart(44100,2,BASS_RECORD_PAUSE,NULL,NULL);
		printf("\nAUDIO: %s",_GetErrorString());
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
	FILE* file;
	file = fopen(filename,"rb");
	long fileLength,offset;
	fseek(file,0,SEEK_END);
	fileLength=ftell(file);
	fseek(file,0,SEEK_SET);
	offset=0;
	fclose(file);
//	delete file;
	if(mode==LOAD_3D)
	{
		return BASS_StreamCreateFile(false, filename,offset,fileLength,BASS_STREAM_AUTOFREE|BASS_SAMPLE_LOOP|BASS_SAMPLE_MONO|BASS_SAMPLE_3D);
		BASS_Apply3D();
	}
	else if(mode==LOAD_2D)
		return BASS_StreamCreateFile(false, filename,offset,fileLength,BASS_STREAM_AUTOFREE|BASS_SAMPLE_LOOP);
}

DWORD
_getChannelData(DWORD channel,int mode,int size,void* buffer)
{
	DWORD flags = mode==FFT? size==256? BASS_DATA_FFT256:size==512?BASS_DATA_FFT512:size==1024?BASS_DATA_FFT1024:size: mode==BYTES?size:BASS_DATA_FLOAT;
	return BASS_ChannelGetData(channel,buffer,flags);
}

HCHANNEL
BassAudio::Loade3DSample(const char* filename)
{
	FILE* file;
	file = fopen(filename,"rb");
	long fileLength,offset;
	fseek(file,0,SEEK_END);
	fileLength=ftell(file);
	fseek(file,0,SEEK_SET);
	offset=0;
	HSAMPLE sample = BASS_SampleLoad(false,filename,offset,0,5,BASS_SAMPLE_LOOP|BASS_SAMPLE_3D);
	std::cout<<_GetErrorString();
	HCHANNEL channel = BASS_SampleGetChannel(sample,true);
	std::cout<<_GetErrorString();
	BASS_Apply3D();
	std::cout<<_GetErrorString();
	return channel;
}



void*
BassAudio::GetChannelBuffer(DWORD channel,int sizeInByte)
{
	void* buffer;
	_getChannelData(channel,(int)BYTES,sizeInByte,buffer);
	return buffer;
}

unsigned
BassAudio::GetChannelFFT(DWORD channel,void* buffer)
{
	return _getChannelData(channel,(int)FFT,(int)Small,buffer);
}

unsigned
BassAudio::GetChannelFFT(DWORD channel,void* buffer,FFT_SIZE size)
{
	return _getChannelData(channel,(int)FFT,(int)size,buffer);
}

void*
BassAudio::GetBackgroundAudioFFT(int size)
{
	void* buffer;
	GetChannelFFT(derAudio,buffer,(FFT_SIZE)size);
	return buffer;
}

void*
BassAudio::GetBackgroundAudioFFT(void)
{
	return GetBackgroundAudioFFT(Small);
}

BassAudio::BassAudio(void)
{
	initDebug();
	BASS_Init(-1,44100,BASS_DEVICE_3D|BASS_DEVICE_FREQ,0,NULL);
	HPLUGIN plgnBASSFX = BASS_PluginLoad("bass_fx.dll",NULL);
	HPLUGIN plgnBASSMIX = BASS_PluginLoad("bassmix.dll",NULL);
	BASS_SetConfig(BASS_CONFIG_UPDATEPERIOD, 100);	
	std::cout<<_GetErrorString();
	BASS_SetEAXParameters(EAX_PRESET_ARENA);
	std::cout<<_GetErrorString();
}


BassAudio::~BassAudio(void)
{
	delete bassaudioInstance;
}

BassAudio*
BassAudio::GetInstance(void)
{
	if(!bassaudioInstance)
		bassaudioInstance = new BassAudio();
	return
		bassaudioInstance;
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
BassAudio::LoadeMusic(const char* fileName,LOAD_MODE mode)
{
	return _getAudioStreamByFileName((const string)fileName,(int)mode);
}
	

void 
BassAudio::SetListenerPosition(TransformA* cameraTranform)
{
	BASS_Set3DPosition(cameraTranform->position.asBassVector(),&cameraTranform->movement,cameraTranform->forward.asBassVector(),cameraTranform->up.asBassVector());
	printf("\nAUDIO: %s",_GetErrorString());
	BASS_Apply3D();
}

void
BassAudio::LoadeBackgroundAudio(const char* fileName)
{
	derAudio = _getAudioStreamByFileName((string)fileName,LOAD_2D);
	BASS_ChannelSetDevice(derAudio,-1);
	printf("\nAudio: %s loadet as Backgroundmusic !\n",fileName);
	_backgroundAudioLoadet = true;
}

void
BassAudio::Play(void)
{
	if(_backgroundAudioLoadet)
	{
		if(BASS_ChannelPlay(derAudio,true))
		{
			_IsPlaying=true;
			std::cout<<"\nAudio:  PLAYING\n";
		}
		else
			printf("Audio: %s",_GetErrorString());
	}
}

void 
BassAudio::Pause(void)
{
	if(_IsPlaying)
	{
		BASS_ChannelPause(derAudio);
		_IsPlaying= false;
		std::cout<<"\nAudio:  PAUSED\n";
	}
}

float
BassAudio::Volume(float vol)
{
	if (vol<=1)
		BASS_SetVolume(vol);
	
	return BASS_GetVolume();
}

void*
BassAudio::GetMasterOutFFT(void)
{
	void* buffer;
	if(_IsRecordingMaster)
		GetChannelFFT(MasterOutResample,buffer,Small);
	printf("\nAUDIO: %s",_GetErrorString());
	return buffer;
}


