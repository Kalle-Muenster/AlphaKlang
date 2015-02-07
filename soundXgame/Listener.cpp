#include "Listener.h"
#include "projectMacros.h"







Listener::Listener(void)
{
	this->IsActive = false;
	splitA = 4;
	splitB = 96;
	MidRange = 0;

	MotivatorA.SetUserMode<SineControlled<float>>(-1,1,0,0.001);
	MotivatorB.SetUserMode<SineControlled<float>>(-100,100,0,1);

	follow.SetUserMode<Follower<float>>(-1,1,0,0.001f);
	follow.GetUserMode<Follower<float>>()->pTARGET = &MidRange;
	Moderator.SetUp(-1,1,0,0,Moderator.Moderate);

	MotivatorA.ControllerActive=false;
	MotivatorB.ControllerActive=false;
	follow.ControllerActive = false;
	Moderator.ControllerActive = false;


}


Listener::~Listener(void)
{
}


bool
Listener::Initialize(void)
{
	UpdateManager::getInstance()->SignInForEarlyUpdate(this);
	return true;
}

void
Listener::GetFFTData(void)
{

		AUDIO->GetChannelFFT(channel,fftData,FFT_SIZE::Small);
		dataRecieved = true;

}

void
Listener::DoEarly(void)
{
	if(IsActive)
	{
	GetFFTData();
	}
}

void
Listener::DoUpdate(void)
{
	if(dataRecieved)
	{
		  short i;
		  Moderator.SetMOVE(MotivatorA);
		  for(i=1;i<splitA;i++)
		    Moderator+=fftData[i];
		  MotivatorA.MOVE = Moderator.Check();
		  Moderator.SetMOVE(follow.Check());
		  for(;i<splitB;i++)
			Moderator+=fftData[i];
		  MidRange = Moderator.Check();
		  Moderator.SetMOVE(MotivatorB);
		  for(;i<128;i++)
			Moderator+=fftData[i];
		  MotivatorB.MOVE = Moderator.Check();
		  dataRecieved=false;

		  printf("MotivatorA: %f  MotivatorB: %f  follow: %f  MidRange: %f",MotivatorA,MotivatorB,follow,MidRange);
	}
}

void
Listener::SetListenToChannel(HCHANNEL AudioChannel)
{
	channel = AudioChannel;
	IsActive = true;
}