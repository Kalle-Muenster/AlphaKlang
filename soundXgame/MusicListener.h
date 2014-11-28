#ifndef _MUSIKLISTENER_H_
#define _MUSIKLISTENER_H_

#include "imusicinteractor.h"

class MusicListener :
	public AMusicInteractor,
	public IConnectable
{

public:
	MusicListener(void)
	{
		DataField=Vector3(0,0,0);

		Line[0].enabled = true;
		Line[0].clampf = true;
		Line[0].MINClampf = 5.f;
		Line[0].MAXClampf = 55.f;
		Line[0].threshold = 1;
		Line[0].fallOff = 1.5f;
		Line[0].lowerBound = 0;
		Line[0].upperBound = 4;
		Line[0].bandWidth = 3;
	}
	virtual ~MusicListener(void){}

protected:
	float value;
	Vector3 DataField;
	virtual float* GettFFTData(void)
		{return (float*)AUDIO->GetBackgroundAudioFFT(FFT_SIZE::Small);}
	virtual void MotivatorFunction(float Motivator,int number) 
		{Vector3 temp;
		//	printf("MusikListener: motivatornumber-%i: %f\n",number,Motivator);
			switch(number)
			{
			case 0:
				//printf("MusikListener: motivator0: %f\n",motivator[0]);
		//		this->Connection()->scale(motivator[0],(motivator[1]+1.5)*2.5f,(motivator[2]+1.5)*2.5f);
				DataField = Connection()->getTransform()->scale;
				DataField.x=Motivator/3.32;
				value = Motivator<40?Motivator<10?0.1f:0.75f:Motivator>48?1.6f:0.5f;
				break;
			case 1:
				temp = this->Connection()->getTransform()->rotation;
				DataField.y=(Motivator+1.5f);
				temp.x += Motivator*value/0.8;
				temp.y -= Motivator*value/2;
				temp.z += Motivator*value/1.5;
				this->Connection()->rotate(temp);
				
				break;
			case 2:
				temp = this->Connection()->getTransform()->movement;
				temp.x *= Motivator/5;
				temp.y *= -Motivator/2;
				temp.z *= Motivator/3;
				this->Connection()->move(temp);

				DataField.z=(Motivator+1.5f);
				this->Connection()->scale(DataField);

				this->GetLineData(0)->fallOff=Motivator>0.75f?0.7f:2.8f;
				this->GetLineData(0)->threshold=Motivator>0.75f?0.6f:value/3.3f;
				break;
			}
		}
	virtual void MotivatorFunction(float Motivator[]){}
};

#endif