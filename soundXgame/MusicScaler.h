#ifndef _CMUSICSCALER_
#define _CMUSICSCALER_

#include "musiccontroller.h"
#include "AudioObject.h"



class MusicScaler :
	public MusicController
{
private:
	Vector3 vec;

public:
	MusicScaler(void);
	virtual bool Initialize(void);
	virtual float* GetFFTData(void);
	virtual ~MusicScaler(void);
	virtual void MotivatorFunction(float Motivator,int number);
	//virtual void MotivatorFunction(float[]);
};

#endif