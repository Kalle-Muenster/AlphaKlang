
#include "MusicScaler.h"



MusicScaler::MusicScaler(void)
{
	Line[0].fallOff = 0.000001f;
}

bool
MusicScaler::Initialize(void)
{
	 return true;
}

MusicScaler::~MusicScaler(void)
{

}

float*
MusicScaler::GetFFTData(void)
{
	return (float*) this->Connection()->GetConnected<AudioEmitter>()->GetFFTData();
}

void
MusicScaler::MotivatorFunction(float Motivator,int number)
{
	if(number==0)
	{	
		Motivator+=5;
		Motivator/=3;

		((ILocatable*)this->Connection())->scale(Motivator,Motivator,Motivator);
	}
}
