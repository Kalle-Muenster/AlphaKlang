#ifndef _MUSIKINTERACTOR_H_
#define _MUSIKINTERACTOR_H_

#include "MusicListener.h"
#include "AudioObject.h"

class MusicInteractor :
	public MusicListener,
	public IConnectable
{

public:
	MusicInteractor(void)
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
	virtual ~MusicInteractor(void){}

protected:
	float timer;
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
				DataField.x=Motivator/3.3;
				value = Motivator<40?Motivator<10?1.f:2.5f:Motivator<50?4.f:Motivator>54?2.f:6.f;
				if(timer>0)
				{	
					timer -= INPUT->FrameTime;
					if(timer<0)
						timer=0;
				}
				if(Motivator>20 && timer==0)
				{
					this->Connection()->GetConnected<AudioEmitter>()->PlayAudio(true);
					timer=0.25;
				}
				break;
			case 1:
				temp = this->Connection()->getTransform()->rotation;
				DataField.y=(Motivator+1.5f)/3;
		//		this->Connection()->GetConnected<AudioEmitter>()->AudioVolume((Motivator+1)*0.5);
				temp.x += Motivator*value/2;
				temp.y -= Motivator*value/2.2;
				temp.z += Motivator*value/1.8;
				this->Connection()->rotate(temp);
				
				break;
			case 2:
				temp = this->Connection()->getTransform()->movement;
				temp.x *= Motivator/3;
				temp.y *= -Motivator/3;
				temp.z *= Motivator/3;
				this->Connection()->move(temp);

				DataField.z=(Motivator+1.5f)/3;
				this->Connection()->scale(DataField);

				this->GetLineData(0)->fallOff=Motivator>0.75f?5.f:1.f;
				this->GetLineData(0)->threshold=Motivator>0.25f?value/24.f:value/2.5f;
			}
		}
	virtual void MotivatorFunction(float Motivator[]){}
};

#endif