#ifndef _CMUSICSCALER_
#define _CMUSICSCALER_

#include "musiclistener.h"
#include "AudioObject.h"



class MusicScaler :
	public MusicListener,
	public IConnectable
{
public:
	MusicScaler(void);
	virtual bool Initialize(void);
	virtual float* GetFFTData(void)
	{
		return (float*) this->Connection()->GetConnected<AudioEmitter>()->GetFFTWindow(FFT_SIZE::Small);
	}
	virtual ~MusicScaler(void);
	virtual void MotivatorFunction(float Motivator,int number)
	{
		if(number==0)
		{	
			Motivator+=10;
			Motivator/=3;
			((ILocatable*)this->Connection())->scale(Motivator,Motivator,Motivator);
		}
	}
protected:
	Vector3 vec;
};

#endif