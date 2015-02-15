#ifndef _CMUSICSCALER_
#define _CMUSICSCALER_

#include "musiccontroller.h"
#include "AudioObject.h"



class MusicScaler :
	public MusicController
{
protected:
	Vector3 positionOffset;
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
			float col = (Motivator*100);
			col=col>255?255:col;
			Connection()->color.byte[1]= (unsigned char)col;
			col = (Motivator*120);
			col=col>255?255:col;
			Connection()->color.byte[2]= (unsigned char)col;
			col = (Motivator*200);
			col=col>255?255:col;
			Connection()->color.byte[3]= (unsigned char)col;
		
			((ILocatable*)this->Connection())->scale(Motivator,Motivator,Motivator);
		}
		//if(number==1)
		//{
		//   Vector3 vec = Connection()->getTransform()->position - positionOffset;
		//   positionOffset = Vector3(getTransform()->scale.x,Motivator,getTransform()->scale.x);
		//   Connection()->move(vec+positionOffset);
		//}
		//if(number==2)
		//{
		//   Connection()->rotate(getTransform()->rotation+Vector3(getTransform()->scale.x,positionOffset.y,Motivator));	
		//}
	}
protected:
	Vector3 vec;
};

#endif