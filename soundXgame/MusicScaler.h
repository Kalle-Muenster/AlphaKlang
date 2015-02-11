#ifndef _CMUSICSCALER_
#define _CMUSICSCALER_

#include "musiccontroller.h"
#include "AudioObject.h"



class MusicScaler :
	public MusicController
{
public:
	MusicScaler(void);
	virtual bool Initialize(void);
	virtual float* GetFFTData(void)
	{
		return (float*) this->Connection()->GetConnected<AudioEmitter>()->GetFFTData();
	}
	virtual ~MusicScaler(void);
	virtual void MotivatorFunction(float Motivator,int number)
	{
		if(number==0)
		{	
			Motivator+=5;
			Motivator/=3;
			((ILocatable*)this->Connection())->scale(Motivator,Motivator,Motivator);
		}
	}
protected:
	Vector3 vec;
};

#endif