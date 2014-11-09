#include "TestYeti.h"
#include "projectClasses.h"

void
_MoveOnMusic(int line,ListenerData* data,IGobject* sender)
{
	((TestYeti*)sender)->MoveOnMusic(line,data);
}

TestYeti::TestYeti(const char* objFile,const char* textureFile,bool addToSceneGraph)
	{ 
		for(int i=0;i<MAXIMUM_NUMBER_OF_CONNECTIONS;i++)
			ConIDs[i]=0;
		this->InitializeObject(objFile,addToSceneGraph);
		this->LoadTexture(textureFile);
		MoveIt = &_MoveOnMusic;

		SetName("Yeti");

	conXtor->AddConnectable<YetiInteractive>();
	conXtor->AddConnectable<MusicListener>();
	
	conXtor->GetConnected<MusicListener>()->SetThreshold(0,0.038f);
	conXtor->GetConnected<MusicListener>()->GetLineData(0)->fallOff=0.1;
	conXtor->GetConnected<MusicListener>()->SetClambt(0,-0.5,10);
	conXtor->GetConnected<MusicListener>()->SetLineBounds(0, 0, 10, 10);

	conXtor->GetConnected<MusicListener>()->Enabled(1,true);
	conXtor->GetConnected<MusicListener>()->SetClambt(1,-1,1);
	conXtor->GetConnected<MusicListener>()->GetLineData(1)->Effect = 0;
	conXtor->GetConnected<MusicListener>()->SetThreshold(1,0.0005f);
	conXtor->GetConnected<MusicListener>()->SetLineBounds(1,64,128,32);
	conXtor->GetConnected<MusicListener>()->GetLineData(1)->fallOff=0.005;

	conXtor->GetConnected<MusicListener>()->AddListenerCallback(0,MoveIt,this);
	conXtor->GetConnected<MusicListener>()->AddListenerCallback(1,MoveIt,this);

	INPUT->attachKey(conXtor->GetConnected<YetiInteractive>());
	INPUT->attachMouseMove(conXtor->GetConnected<YetiInteractive>());	

	//conXtor->GetConnected<AudioEmitter>()->PlayAudio();

	AUDIO->MasterResampling(true);
	}

TestYeti::~TestYeti(void)
{
	
}

void
TestYeti::DoUpdate(void)
{
	if(this->HasConnected<AudioEmitter>())
	{
	if(conXtor->GetConnected<AudioEmitter>()->IsAudioPlaying())
	{
		float* fftWin = conXtor->GetConnected<AudioEmitter>()->GetFFTWindow(FFT_SIZE::Small);
		conXtor->GetConnected<MusicListener>()->listen(fftWin);
	}	
	}
}

void
TestYeti::MoveOnMusic(int line ,ListenerData* data)
{
	float lowerV0;
	float effect0,effect1;
	float threshold0;
	float lowerV1;

	switch(line)
	{
	case 0:
		effect0 = data->Effect;
		threshold0 = data->threshold;
		lowerV0  = data->value[0];

		rotate(transform.rotation.x,(transform.rotation.y+=effect0)+20*((lowerV0<threshold0)? -lowerV0 : lowerV0),getTransform()->rotation.z);
		break;

	case 1:
		effect0 = conXtor->GetConnected<MusicListener>()->GetLineData(0)->Effect;
		effect1 = data->Effect;
		lowerV1  = data->value[0];

		rotate(transform.rotation.x,transform.rotation.y,effect0*2<1? lowerV1*45:getTransform()->rotation.z);

		if(effect1>=conXtor->GetConnected<MusicListener>()->GetLineData(1)->MAXClampf-0.1)
		{
			move(transform.position.x,transform.position.y+effect1/6,transform.position.z);
			data->threshold=0.02f;
		}
		else if(transform.position.y>0)
		{
			if(effect1 < 0)
		{
			
			move(transform.position.x,transform.position.y+effect1,transform.position.z);
			effect1-=0.2f;
			
		}
			else
			effect1-=0.3f;
			move(transform.position.x,transform.position.y-effect1/10,transform.position.z);
		}
		else
		{
			data->threshold=0.0005;
		}

	//	printf("\nline1: %f  Y: %f ",effect1,transform.position.y);
		break;
	}

}