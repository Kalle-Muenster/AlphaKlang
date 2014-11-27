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
	virtual void MotivatorFunction(int number) 
		{Vector3 temp;
			switch(number)
			{
			case 0:
				//printf("MusikListener: motivator0: %f\n",motivator[0]);
				this->Connection()->scale(motivator[0],(motivator[1]+1.5)*2.5f,(motivator[2]+1.5)*2.5f);
				break;
			case 1:
				//printf("MusikListener: motivator1: %f\n",motivator[1]);
				temp = this->Connection()->getTransform()->rotation;
				temp.x += motivator[1];
				temp.y -= motivator[1];
				temp.z += motivator[0]/5.f;
				this->Connection()->rotate(temp);
				break;
			case 2:
				//printf("MusikListener: motivator2: %f\n",motivator[2]);
				temp = this->Connection()->getTransform()->position;
				temp.x += motivator[2]/5;
				temp.y += -motivator[2]/3;
				temp.z += motivator[2]/4;
				this->Connection()->move(temp);
				break;
			}
		}
};

#endif