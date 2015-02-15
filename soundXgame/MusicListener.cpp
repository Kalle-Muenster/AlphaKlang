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
		Line[i].Effect.SetUp(0,0,0.5f,0.001f,Line[i].Effect.Damp);
		Line[i].Effect = 0.5f;
		Line[i].Effect.ControllerActive = true;
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

//Enable and Disable..
bool
MusicListener::Enabled(int lineNumber,BOOL enable)
{
	if(enable<3)
		Line[lineNumber].enabled = enable;

	return Line[lineNumber].enabled;
}


//calculates the Motivator-Value (output) for the given "line":..
float
MusicListener::listenTo(int line, float* fft)
{
	//return if no fft-data available
	if(!FFTdataValide)
		return 0;

	float lowVal=0;
	float highVal=0;

	//calculate the average level of the lower bands
	for(int c1 = Line[line].lowerBound; c1 < Line[line].lowerBound + Line[line].bandWidth; c1++)
		lowVal += fft[c1];
	lowVal /= Line[line].bandWidth;

	//calculate half the average level of the higher bands
	for(int c2 = Line[line].upperBound - Line[line].bandWidth; c2 < Line[line].upperBound; c2++)
		highVal += fft[c2];
	highVal /= (Line[line].bandWidth/2);

	//apply effect to the output variable...
	calculateEffect(line,lowVal,highVal);

	//if clamping is activated the output-value will be clambt to the MIN/MAX values.
	if(Line[line].clampf)
	{
		//if automatic-falloff adjust is activated:...
		if(automaticFallOffAdjust)
		{
			if(Line[line].Effect<Line[line].MINClampf)
			{	//the "falloff"-value will be set to a lower value when output-variable got under the MIN-Clamp-mark, to make the value falling slower the next time
				Line[line].fallOff -= (((1-((Line[line].MINClampf-Line[line].Effect)/Line[line].fallOff ))/2) * Line[line].fallOff);
				//then the value will be clambt.
				Line[line].Effect = Line[line].MINClampf;

			}
			else if(Line[line].Effect>Line[line].MAXClampf)
			{	//the "falloff" will be set to a higher value when reached MAX-Clamp-mark, to make the output-value falling back faster the next time
				Line[line].fallOff += (((  ((Line[line].Effect-Line[line].MAXClampf)/Line[line].fallOff ))/2) * Line[line].fallOff);
				//and clamp it!
				Line[line].Effect = Line[line].MAXClampf;
			}
		}
		else   //if no automatic-falloff-adjust, just clamp to MIN/MAX..
			Line[line].Effect=Line[line].Effect<Line[line].MINClampf?Line[line].MINClampf:Line[line].Effect>Line[line].MAXClampf?Line[line].MAXClampf:Line[line].Effect;
	}

	//save low and high average values (they may be useful when using the output in the motivator-function..)
	Line[line].value[0]=lowVal;
	Line[line].value[1]=highVal;

	//and return the output...
	return Line[line].Effect;
}

//apply the effect to the output variable
void
MusicListener::calculateEffect(int line,float lowValue,float highValue)
{
	//if the average low-band-level becomes higher than "threshold" 
	//high-band-level multiplied by Low-band-level multiplied by "sensitivity" will be added.
	if(lowValue > Line[line].threshold - highValue)
		Line[line].Effect = Line[line].Effect + highValue*sensitivity*lowValue;
	else  // if not over the threshold, "fallOff" will be subtracted.
		Line[line].Effect = Line[line].Effect - Line[line].fallOff;
}

//calculate the output variables for each "line" which is enabled..
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


//retrieve fft-data from the in GetFFTData() defined audio-source at Early-Update. 
void 
MusicListener::DoEarly(void)
{
	void * validation = GetFFTData();
	FFTdataValide = (bool*)validation;
	listen((float*)validation);
}

//call the Motivator-functions at Standard-Update
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

//get accsess to a requested fx-line
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

//set the output of line "line" to be clambt or not!
void
MusicListener::SetClambt(int line,bool clamb)
{
	Line[line].clampf=clamb;
}

//set a line clambt and set its MIN/MAX values.
void
MusicListener::SetClambt(int line,float min,float max)
{
	Line[line].clampf=true;
	Line[line].MINClampf = min;
	Line[line].MAXClampf = max;
	Line[line].Effect.MOVE = (max-min)/100;
} 

//set a line's band-ranges..
//it means:	start mesuaring the lower bands at "lower" to "lower"+"width"
//and the upper bands from "upper"-"width" to "upper". (so lower will be bottom and upper will be top of overall band-range) 
void
MusicListener::SetLineBounds(int line,int lower,int upper,int width)
{
	Line[line].bandWidth = width;
	Line[line].lowerBound = lower;
	Line[line].upperBound = upper;
}

//empty "Motivator-function" to be overriden and placed code in for doing movement or other changing properties in derived classes..
void
MusicListener::MotivatorFunction(float motivator,int number)
{

}

//another empty motivator-function (gets all enabled motivators at once,when all of them have been calculated for this frame.. 
//and not each after another like the function before)
void
MusicListener::MotivatorFunction(float[])
{

}
