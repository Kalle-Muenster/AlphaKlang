#include "SpectrumAnalyzer.h"
#include "projectMacros.h"
#include "Connectable.h"

SpectrumAnalyzer::SpectrumAnalyzer(void)
{
	IGObject::InitializeObject();
	this->ChartHeight = 50.0f;
}

void
SpectrumAnalyzer::Initialize(void)
{
	//Setting up a color...
	color.byte[1]=0;
	color.byte[2]=255;
	color.byte[3]=127;
	color.byte[0]=60;

	// Setting up an offset-vector 
	// (will be addet to each Subobject's vertices)  
	Vector3 offset = this->getTransform()->position;
	offset.y += 0.5;
	float X = offset.x;
	X-=(SPECTRUM_SIZE*0.5);
	
	// Instanciate as many Cubes as there are frequency-bands 
	// in the used fft-window... for each, increase its possition-offset
	// by its size allong the offset-direction.
	for(int i = 0; i<SPECTRUM_SIZE;i++)
	{
		offset.x = X + i;
		bands[i]=(new Cubus(color,true,false,offset));
		bands[i]->IsGrounded(true);
		bands[i]->GroundedWithPivot = true;
		bands[i]->scale(getTransform()->scale);
	}
	
	// reset movement every step you select it for executing with bands
	transform.movement = *Vector3::Zero;
	
	/* Setting the Meters "FallOff"...
	 * as lower the value, as slower the visuals will
	 * Fall back to Zero if there's no signal. */
	fallOffAmount = 0.05f / 3.0f * this->ChartHeight;
	
	// this set's the hight of the analyzers chart.
	// band meshures will multiplied by it.
	this->getTransform()->scale.y = this->ChartHeight;

	//get beeing invoked at EarlyUpdate, to get the FFT-Data...
	UPDATE->SignInForEarlyUpdate(this);

}

void
SpectrumAnalyzer::DoEarly(void)
{ // Get The FFT-Datta-Buffer from desired audio-channel.
  // ...in this case the Backgroundaudio channel....
	this->fftData = (float*)AUDIO->GetBackgroundAudioFFT(FFT_SIZE::Small);
}

void
SpectrumAnalyzer::DoUpdate(void)
{//Reading out the Early-getted fft-buffer and aply
 //it's values on the Sub-Cubes....
	float changeFactor;
	for(int i=0;i<SPECTRUM_SIZE;i++)
	{
		//scaling datavalues to the desired size...
		//also let's flatten the scale and maybe do some compression 
		//to make the chart look's more strait, and the bands 
		//look'n more equivalent then real spectrum-data will do...  

	//	changeFactor = (this->fftData[i]*((float)i*0.1));
		changeFactor = (this->fftData[i]*((float)1+i*0.128));         
		ChangeSize(i,changeFactor * this->getTransform()->scale.y);
		ChangeColor(i,changeFactor);
	}
}

void
SpectrumAnalyzer::DoLate(void)
{
	//maybe the sub-updates here better...
}

void 
SpectrumAnalyzer::ChangeColor(int index,float factor)
{
	//Do flashing the collors.....
	color.byte[1] = ((factor*10) * 255.0);
	color.byte[2] = (1.0 - (factor*10))*255.0;
	bands[index]->SetColor(color);
}
void
SpectrumAnalyzer::ChangeSize(int band,float newScaleY)
{   
	//apllie the measured and compressed signalvalues to the
	//Subcube's Transform scale's if the values have changed...

	//if the new value is lower than the old value,
	//shrink the cube's Y-Axis by the "Fallback"-value...
	if(newScaleY < bands[band]->getTransform()->scale.y)
		bands[band]->getTransform()->scale.y -= fallOffAmount;
	else  // if th new value is higher than the old one, just set it's Y-scale to the new value...
		bands[band]->getTransform()->scale.y = newScaleY;
}

void
SpectrumAnalyzer::draw(void)
{
	if(!IsVisible)
		return;

	if(_CHANGEDposition || _CHANGEDrotation || _CHANGEDscale)
	{
		// Re-Positioning,sizeing and rotating the SubCubes
		// if the whole analyzerobject has been moved,resized 
		// or rotated during last update..  -->> maybe this coul'd be don at LateUpdate better... dont know....
		for(int i=0;i<SPECTRUM_SIZE;i++)
		{
			if(_CHANGEDposition)
				bands[i]->getTransform()->position += transform.movement;
				//bands[i]->getTransform()->position += offset;
			if(_CHANGEDrotation)
				bands[i]->getTransform()->rotation = getTransform()->rotation;
			if(_CHANGEDscale)
				bands[i]->scale(transform.scale.x,bands[i]->getTransform()->scale.y,transform.scale.z);
		}
		_CHANGEDposition = _CHANGEDrotation = _CHANGEDscale = false;
		transform.movement = *Vector3::Zero;
	}

	//draw each subcube....
	for(int i=0;i<SPECTRUM_SIZE;i++)
	{
		//bands[i]->getTransform()->position.y = Ground::getInstance()->GetGroundY(bands[i]->getTransform()->position.x, bands[i]->getTransform()->position.z);
		bands[i]->draw();
	}
}

Vector3
SpectrumAnalyzer::move(float X,float Y,float Z)
{
	IMeshObject::move(X,Y,Z);
	_CHANGEDposition=true;
	return getTransform()->position;
}

Vector3
SpectrumAnalyzer::rotate(float X,float Y,float Z)
{
	_CHANGEDrotation=true;
	return IMeshObject::rotate(X,Y,Z);
}

// scale this object to the size each subobject will have.. 
Vector3
SpectrumAnalyzer::scale(float X,float Y,float Z)
{
	_CHANGEDscale=true;
	return IMeshObject::scale(X,Y,Z);
}

SpectrumAnalyzer::~SpectrumAnalyzer(void)
{   // dont forget signing out from the
	// Events we're registerd to if destructing...
	UPDATE->SignOutFromEarlyUpdate(this);
	UPDATE->SignOutFromUpdate(this);
	UPDATE->SignOutFromLateUpdate(this);
	delete[] bands;
	delete fftData;
}