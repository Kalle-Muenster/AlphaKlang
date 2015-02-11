//#include "Connectable.h"
#include "MusicListener.h"







MusicListener::MusicListener(void)
{
	automaticFallOffAdjust=true;
	FFTdataValide = false;
	allMotivatorsEnabled  = false;
	motivatorsUpdated = false;
	sensitivity = 50;

	for(int i=0;i<NUMBER_OF_LISTENINGLINES;i++)
	{
		Line[i].enabled = false;
		Line[i].threshold = 0.02f;
		Line[i].Effect = 0.5f;
		Line[i].lowerBound = 1;
		Line[i].upperBound = 64;
		Line[i].bandWidth = 32;
		Line[i].clampf = true;
		Line[i].MINClampf = -1.0f;
		Line[i].MAXClampf = 1.0f;
		Line[i].fallOff = 0.05f;
		for(int c=0;c<NUMBER_OF_LINEVALUES;c++)
			Line[i].value[c] = 0;
	}

	Line[0].enabled = true;
	Line[0].clampf = true;
	Line[0].MINClampf = -1;
	Line[0].MAXClampf = 1;
	Line[0].threshold = 0.33;
	Line[0].fallOff = 0.025f;
		Line[0].lowerBound = 0;
		Line[0].upperBound = 4;
		Line[0].bandWidth = 3;

	Line[1].enabled = true;
	Line[1].clampf = true;
	Line[1].MINClampf = -1.0f;
	Line[1].MAXClampf = 1.0f;
	Line[1].lowerBound = 32;
	Line[1].upperBound = 96;
	Line[1].fallOff = 0.0075f;

	Line[2].enabled = true;
	Line[2].clampf = true;
	Line[2].MINClampf = -1.0f;
	Line[2].MAXClampf = 1.0f;
	Line[2].bandWidth = 16;
	Line[2].lowerBound = 64;
	Line[2].upperBound = 96;
	Line[2].fallOff = 0.0025f;

	UPDATE->SignInForEarlyUpdate(this);
}


MusicListener::~MusicListener(void)
{
	UPDATE->SignOutFromEarlyUpdate(this);
	UPDATE->SignOutFromUpdate(this);
}

bool
MusicListener::Enabled(int lineNumber,BOOL enable)
{
	if(enable<3)
		Line[lineNumber].enabled = enable;

	return Line[lineNumber].enabled;
}

float
MusicListener::listenTo(int line, float* fft)
{
	if(!FFTdataValide)
		return 0;

	float lowVal=0;
	float highVal=0;

	for(int c1 = Line[line].lowerBound; c1 < Line[line].lowerBound + Line[line].bandWidth; c1++)
		lowVal += fft[c1];

	for(int c2 = Line[line].upperBound - Line[line].bandWidth; c2 < Line[line].upperBound; c2++)
		highVal += fft[c2];

	lowVal /= Line[line].bandWidth;
	highVal /= (Line[line].bandWidth/2);

	calculateEffect(line,lowVal,highVal);

	if(Line[line].clampf)
	{
		
		if(automaticFallOffAdjust)
		{
			if(Line[line].Effect<Line[line].MINClampf)
			{
				Line[line].fallOff -= (((1-((Line[line].MINClampf-Line[line].Effect)/Line[line].fallOff ))/2) * Line[line].fallOff);
				Line[line].Effect = Line[line].MINClampf;

			}
			else if(Line[line].Effect>Line[line].MAXClampf)
			{
				Line[line].fallOff += (((  ((Line[line].Effect-Line[line].MAXClampf)/Line[line].fallOff ))/2) * Line[line].fallOff);
				Line[line].Effect = Line[line].MAXClampf;
			}
		}
		else
			Line[line].Effect=Line[line].Effect<Line[line].MINClampf?Line[line].MINClampf:Line[line].Effect>Line[line].MAXClampf?Line[line].MAXClampf:Line[line].Effect;
	}
	Line[line].value[0]=lowVal;
	Line[line].value[1]=highVal;

	return Line[line].Effect;
}

void
MusicListener::calculateEffect(int line,float lowValue,float highValue)
{
	if(lowValue > Line[line].threshold - highValue)
		Line[line].Effect+=highValue*sensitivity*lowValue;
	else
		Line[line].Effect-=Line[line].fallOff;
}

float*
MusicListener::listen(float *fft)
{
	allMotivatorsEnabled = true;
	for(int i=0;i<NUMBER_OF_LISTENINGLINES;i++)
		if(Line[i].enabled)
			motivator[i] = listenTo(i,fft);
		else
		{
			motivator[i] = _FNan._Float;
			allMotivatorsEnabled = false;
		}
	motivatorsUpdated=true;
	return &motivator[0];
}

void 
MusicListener::DoEarly(void)
{
	void * validation = GetFFTData();
	FFTdataValide = (bool*)validation;
	listen((float*)validation);
}

void 
MusicListener::DoUpdate(void)
{
	if(motivatorsUpdated)
	{
		for(int i=0;i<NUMBER_OF_LISTENINGLINES;i++)
			if(Line[i].enabled)
				MotivatorFunction(motivator[i],i);
		FFTdataValide=motivatorsUpdated=false;
	}
}

ListenerData*
MusicListener::GetLineData(int number)
{
	return &Line[number];
}


/* trigger, der beim Überschreiten ausgelöst wird */
void
MusicListener::SetThreshold(int line,float value)
{
	Line[line].threshold = value;
}

void
MusicListener::SetClambt(int line,bool clamb)
{
	Line[line].clampf=clamb;
}

void
MusicListener::SetClambt(int line,float min,float max)
{
	Line[line].clampf=true;
	Line[line].MINClampf = min;
	Line[line].MAXClampf = max;
}

void
MusicListener::SetLineBounds(int line,int lower,int upper,int width)
{
	Line[line].bandWidth = width;
	Line[line].lowerBound = lower;
	Line[line].upperBound = upper;
}

void
MusicListener::MotivatorFunction(float motivator,int number)
{

}

void
MusicListener::MotivatorFunction(float[])
{

}
