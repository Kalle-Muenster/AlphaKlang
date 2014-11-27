//#include "Connectable.h"
#include "IMusicInteractor.h"


AMusicInteractor::AMusicInteractor(void)
{
	allMotivatorsEnabled  = false;
	motivatorsUpdated = false;

	for(int i=0;i<NUMBER_OF_LISTENINGLINES;i++)
	{
		Line[i].enabled = false;
		Line[i].threshold = 0.02f;
		Line[i].Effect = 1.0f;
		Line[i].lowerBound = 0;
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
	Line[0].MINClampf = 0.5f;
	Line[0].MAXClampf = 5.5f;
	Line[0].threshold = 0.1;
	Line[0].fallOff = 0.1f;


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


AMusicInteractor::~AMusicInteractor(void)
{
	UPDATE->SignOutFromEarlyUpdate(this);
	UPDATE->SignOutFromUpdate(this);
}

bool
AMusicInteractor::Enabled(int lineNumber,BOOL enable)
{
	if(enable<3)
		Line[lineNumber].enabled = enable;

	return Line[lineNumber].enabled;
}

float
AMusicInteractor::listenTo(int line, float* fft)
{

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
		Line[line].Effect=Line[line].Effect<Line[line].MINClampf?Line[line].MINClampf:Line[line].Effect>Line[line].MAXClampf?Line[line].MAXClampf:Line[line].Effect;

	Line[line].value[0]=lowVal;
	Line[line].value[1]=highVal;

	return Line[line].Effect;
}

void
AMusicInteractor::calculateEffect(int line,float lowValue,float highValue)
{
	if(lowValue > Line[line].threshold - highValue)
		Line[line].Effect+=highValue*50*lowValue;
	else
		Line[line].Effect-=Line[line].fallOff;
}

float*
AMusicInteractor::listen(float *fft)
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
AMusicInteractor::DoEarly(void)
{
	listen(GetFFTData());
}

void 
AMusicInteractor::DoUpdate(void)
{
	if(motivatorsUpdated)
	{
		for(int i=0;i<NUMBER_OF_LISTENINGLINES;i++)
			if(Line[i].enabled)
				MotivatorFunction(motivator[i],i);
		motivatorsUpdated=false;
	}
}

ListenerData*
AMusicInteractor::GetLineData(int number)
{
	return &Line[number];
}


/* trigger, der beim Überschreiten ausgelöst wird */
void
AMusicInteractor::SetThreshold(int line,float value)
{
	Line[line].threshold = value;
}

void
AMusicInteractor::SetClambt(int line,bool clamb)
{
	Line[line].clampf=clamb;
}

void
AMusicInteractor::SetClambt(int line,float min,float max)
{
	Line[line].clampf=true;
	Line[line].MINClampf = min;
	Line[line].MAXClampf = max;
}

void
AMusicInteractor::SetLineBounds(int line,int lower,int upper,int width)
{
	Line[line].bandWidth = width;
	Line[line].lowerBound = lower;
	Line[line].upperBound = upper;
}

