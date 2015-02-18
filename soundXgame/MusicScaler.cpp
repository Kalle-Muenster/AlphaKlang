
#include "MusicScaler.h"



MusicScaler::MusicScaler(void)
{
//	Line[0].fallOff = 0.000001f;
}

bool
MusicScaler::Initialize(void)
{
	 return true;
	 vec = *Vector3::Zero;
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
		vec.x = Motivator*2;
	}
	if(number==1)
	{
		vec.y=Motivator*5;
	}
	if(number==2)
	{
		vec.z=Motivator*10;
		Connection()->scale(vec);
	}
}

