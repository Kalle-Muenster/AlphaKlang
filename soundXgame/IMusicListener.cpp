#include "IMusicListener.h"


IMusicListener::IMusicListener(void)
{
	for(int i=0;i<NUMBER_OF_LISTENINGLINES;i++)
	{
		Line[i].enabled = false;
		Line[i].threshold = 0.033f;
		Line[i].Effect = 1.0f;
		Line[i].lowerBound = 0;
		Line[i].upperBound = 96;
		Line[i].bandWidth = 32;
		Line[i].clampf = false;
		Line[i].MINClampf = -_FInf._Float;
		Line[i].MAXClampf = _FInf._Float;
		Line[i].fallOff = 0.1f;
		for(int c=0;c<NUMBER_OF_LINEVALUES;c++)
			Line[i].value[c] = 0;
	}
	Line[0].enabled = true;
	Line[0].clampf = true;
	Line[0].MINClampf = -0.5f;
	Line[0].MAXClampf = 10.0f;


}


IMusicListener::~IMusicListener(void)
{
}

bool
IMusicListener::Enabled(int lineNumber,BOOL enable)
{
	if(enable<3)
		Line[lineNumber].enabled = enable;

	return Line[lineNumber].enabled;
}

float
IMusicListener::listenTo(int line,float *fft)
{

		float lowVal=0;
		float highVal=0;
	

		for(int c1=Line[line].lowerBound;c1<Line[line].lowerBound+Line[line].bandWidth;c1++)
			lowVal += fft[c1];

		for(int c2=Line[line].upperBound-Line[line].bandWidth;c2<Line[line].upperBound;c2++)
			highVal += fft[c2];

		lowVal/=Line[line].bandWidth;
		highVal/=(Line[line].bandWidth/2);

		applyEffect(line,lowVal,highVal);

		if(Line[line].clampf)
			Line[line].Effect=Line[line].Effect<Line[line].MINClampf?Line[line].MINClampf:Line[line].Effect>Line[line].MAXClampf?Line[line].MAXClampf:Line[line].Effect;


		Line[line].value[0]=lowVal;
		Line[line].value[1]=highVal;

		if(this->callbacks[line])
			callbacks[line](line,&Line[line],clients[line]);

		return Line[line].Effect;
}

void
IMusicListener::applyEffect(int line,float lowValue,float highValue)
{
	if(lowValue > Line[line].threshold - highValue)
		Line[line].Effect+=highValue*50*lowValue;
	else
		Line[line].Effect-=Line[line].fallOff;
}

float*
IMusicListener::listen(float *fft)
{
	float lineResults[NUMBER_OF_LISTENINGLINES];
	for(int i=0;i<NUMBER_OF_LISTENINGLINES;i++)
		if(Line[i].enabled)
			lineResults[i] = listenTo(i,fft);
		else
			lineResults[i] = _FNan._Float;

	return &lineResults[0];
}

ListenerData*
IMusicListener::GetLineData(int number)
{
	return &Line[number];
}


void
IMusicListener::SetThreshold(int line,float value)
{
	Line[line].threshold = value;
}

void
IMusicListener::SetClambt(int line,bool clamb)
{
	Line[line].clampf=clamb;
}

void
IMusicListener::SetClambt(int line,float min,float max)
{
	Line[line].clampf=true;
	Line[line].MINClampf = min;
	Line[line].MAXClampf = max;
}

void
IMusicListener::SetLineBounds(int line,int lower,int upper,int width)
{
	Line[line].bandWidth = width;
	Line[line].lowerBound = lower;
	Line[line].upperBound = upper;
}

void
IMusicListener::AddListenerCallback(int line,ListenerFunc callback,IGobject* client)
{
	this->callbacks[line] = callback;
	this->clients[line] = client;
}