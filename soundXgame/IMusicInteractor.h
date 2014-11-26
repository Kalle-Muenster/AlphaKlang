#ifndef _IMUSICLISTENER_H_
#define _IMUSICLISTENER_H_

#include <math.h>
#include "projectMacros.h"
#include "Connectable.h"

#define NUMBER_OF_LINEVALUES (2)

struct ListenerData
{
	bool enabled;
	float threshold;
	float Effect;
	int lowerBound;
	int upperBound;
	float value[NUMBER_OF_LINEVALUES];
	int bandWidth;
	float fallOff; /* Zeit in sekunden, auf dem der threshold bleiben soll */
	bool clampf; /* clampen min / max begrenzen für end-ausgabe wert */
	float MINClampf;
	float MAXClampf;
};

//typedef void(*ListenerFunc)(int,ListenerData*,IGObject*);
//
//class IMusicListener
//{
//public:
//	
//			IMusicListener(void);
//	virtual ~IMusicListener(void);
//
//			float listenTo(int,float *fft);
//			float* listen(float *fft);
//			ListenerData* GetLineData(int);
//			void SetThreshold(int,float);
//			void SetClambt(int,bool);
//			void SetClambt(int,float,float);
//			bool Enabled(int,BOOL=3);
//			void SetLineBounds(int line,int lower,int upper,int width);
//	
//			
//			
//protected:
//
//};


class AMusicInteractor :  
	public IUpdateble
{
public:
	static const int NUMBER_OF_LISTENINGLINES = 5;

	AMusicInteractor(void);
	virtual ~AMusicInteractor(void);
	ListenerData* GetLineData(int);
	float motivator[NUMBER_OF_LISTENINGLINES];
	void SetThreshold(int,float);
	void SetClambt(int,bool);
	void SetClambt(int,float,float);
	bool Enabled(int,BOOL=3);
	void SetLineBounds(int line,int lower,int upper,int width);

	virtual void DoEarly(void)
	{
		listen(GetFFTData());
	}
	virtual void DoUpdate(void)
	{
		if(motivatorsUpdated)
		{
			for(int i=0;i<NUMBER_OF_LISTENINGLINES;i++)
				if(Line[i].enabled)
					MotivatorFunction(i);
			motivatorsUpdated=false;
		}
	}

protected:
	bool motivatorsUpdated;
	virtual float* GetFFTData(void){return (float*)AUDIO->GetBackgroundAudioFFT(FFT_SIZE::Small);}
	virtual void MotivatorFunction(int lineNumber){}
	float listenTo(int,float *fft);
	float* listen(float *fft);
	virtual void calculateEffect(int line,float lowValue,float highValue);
	ListenerData Line[NUMBER_OF_LISTENINGLINES];
};




#endif