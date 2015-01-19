#include "Listener.h"
#include "projectMacros.h"

Listener::Listener(void)
{
	MotivatorA.SetUserMode<SineControlled<float>>(-1,1,0,0.001);
	MotivatorB.SetUserMode<SineControlled<float>>(-100,100,0,1);
	MotivatorA.ControllerActive=false;
	MotivatorB.ControllerActive=false;
	TriggerA.SetUserMode<Trigger<float>>(0);
	TriggerB.SetUserMode<Trigger<float>>(0);
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
}

void
Listener::DoEarly(void)
{
	GetFFTData();
}

void
Listener::DoUpdate(void)
{
	//calculations...
}

void
Listener::ListenToChannel(HCHANNEL AudioChannel)
{
	channel = AudioChannel;
}