#ifndef _MUSIKLISTENER_H_
#define _MUSIKLISTENER_H_

#include "imusicinteractor.h"

class MusicListener :
	public AMusicInteractor,
	public IConnectable
{

public:
	MusicListener(void){}
	virtual ~MusicListener(void){}

protected:
	virtual float* GettFFTData(void)
		{return (float*)AUDIO->GetBackgroundAudioFFT(FFT_SIZE::Small);}
	virtual void MotivatorFunction(float Motivator,int number) 
		{Vector3 temp2,temp;
			printf("MusikListener: motivatornumber-%i: %f\n",number,Motivator);
			switch(number)
			{
			case 0:
					temp2.x=Motivator/10;
					temp2.y=Motivator/10;
					temp2.z=Motivator/10;
					temp2=temp2-(this->Connection()->getTransform()->movement/5);
					this->Connection()->scale(temp2);
				break;
			case 1:
				temp = this->Connection()->getTransform()->rotation;

				temp.x += Motivator;
				temp.y -= Motivator;
				temp.z += Motivator;
				this->Connection()->rotate(temp);
				
				break;
			case 2:
				temp = this->Connection()->getTransform()->movement;
				temp.x *= Motivator/5;
				temp.y *= -Motivator/3;
				temp.z *= Motivator/7;
				this->Connection()->move(temp);
				
				break;
			}
		}
	virtual void MotivatorFunction(float Motivator[]){}
};

#endif