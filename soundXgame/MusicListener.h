#ifndef _IMUSICLISTENER_
#define _IMUSICLISTENER_

#include <math.h>
#include "projectMacros.h"
#include "Connectable.h"

#define NUMBER_OF_LINEVALUES (2)

struct ListenerData
{
	bool enabled;
	float threshold;	// if the level becomes over this value, the output will raise by this value.
	Controlled<float> Effect;  // the output value. it's a ControlledValue in "Damp" mode which won't allow changes higher then 2% of the MIN/MAX-range.. 
	int lowerBound;	// untere grenze des zu analysierenden spektrums...  
	int upperBound;	 // obere grenze des zu analysierenden spektrums...
	float value[NUMBER_OF_LINEVALUES];
	int bandWidth;
	float fallOff; /* if the level stays under the "threshold" the output will fall by this value  */
	bool clampf; /* clampen min / max begrenzen für end-ausgabe wert */
	float MINClampf;
	float MAXClampf;
	float lowerAmp;	   //amplify the input of the lower bands
	float upperAmp;	   //amplify the input of the upper bands
};




class MusicListener :  
	public IUpdateble
{
public:
	static const int NUMBER_OF_LISTENINGLINES = 5;

	MusicListener(void);
	virtual ~MusicListener(void);
	ListenerData* GetLineData(int number);
	float motivator[NUMBER_OF_LISTENINGLINES];
	void SetThreshold(int line,float value);
	void SetClambt(int line,bool clamb);
	void SetClambt(int line,float min,float max);
	bool Enabled(int lineNumber,BOOL=3);
	void SetInputAplification(int line,float lower,float upper);
	void SetLineBounds(int line,int lower,int upper,int width);
	float sensitivity;
	virtual void DoEarly(void);
	virtual void DoUpdate(void);
	bool automaticFallOffAdjust;
protected:
	bool motivatorsUpdated;
	virtual float* GetFFTData(void)
		{return(float*)AUDIO->GetBackgroundAudioFFT(FFT_SIZE::Small);}
	virtual void MotivatorFunction(float motivator,int number);
	virtual void MotivatorFunction(float[]);
	float listenTo(int,float *fft);
	float* listen(float *fft);
	virtual void calculateEffect(int line,float lowValue,float highValue);
	ListenerData Line[NUMBER_OF_LISTENINGLINES];
	bool allMotivatorsEnabled;
	bool FFTdataValide;
};




#endif