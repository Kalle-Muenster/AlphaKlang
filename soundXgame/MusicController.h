#ifndef _CMUSICCONTROLLER_
#define	_CMUSICCONTROLLER_

#include "MusicListener.h"
#include "AudioObject.h"

class MusicController :
	public MusicListener,
	public IConnectable
{

public:
	MusicController(void)
	{
		DataField=Vector3(0,0,0);
		Line[0].enabled = true;
		Line[0].clampf = true;
		Line[0].MINClampf = 0.0f;
		Line[0].MAXClampf = 100.0f;
		Line[0].threshold = 0.5f;
		Line[0].fallOff = 1.5f;
		Line[0].lowerBound = 10;
		Line[0].upperBound = 37;
		Line[0].bandWidth = 14;

	}
	virtual ~MusicController(void){}

protected:
	float timer;
	float value;
	Vector3 DataField;
	virtual float* GettFFTData(void)
	{
		return (float*)AUDIO->GetBackgroundAudioFFT(FFT_SIZE::Small);
	}
	virtual void MotivatorFunction(float Motivator,int number)
	{
		Vector3 temp;
		if(number == 0)
		{
			
		}
	}
	virtual void MotivatorFunction(float Motivator[]){}
};

#endif