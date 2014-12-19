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
		Line[0].MAXClampf = 10.0f;
		Line[0].threshold = 0.05f;
		Line[0].fallOff = 0.00001f;
		Line[0].lowerBound = 0;
		Line[0].upperBound = 9;
		Line[0].bandWidth = 5;

	}
	virtual ~MusicController(void){}

protected:
	float timer;
	float value;
	Vector3 DataField;
	virtual float* GetFFTData(void)
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