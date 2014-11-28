#include "SpectrumAnalyzer.h"
#include "projectMacros.h"
#include "Connectable.h"

SpectrumAnalyzer::SpectrumAnalyzer(void)
{
	IGObject::InitializeObject();
	this->IsGrounded = false;
	InitializeSpectrumAnalyzer();
}

void
SpectrumAnalyzer::InitializeSpectrumAnalyzer(void)
{
	//Setting up a color...
	color.byte[1]=0;
	color.byte[2]=255;
	color.byte[3]=127;
	color.byte[0]=10;

	// Setting up an offset-vector 
	// (will be addet to each Sybobject's vertices)  
	offset = this->getTransform()->position;
	offset.y += 0.5;
	float X = offset.x;
	X-=(SPECTRUM_SIZE*0.5);

	// scale this object to the size each subobject will have.. 
	this->getTransform()->scale.x=0.3f;
	this->getTransform()->scale.y=0.3f;
	this->getTransform()->scale.z=2.0f;

	// Instanciate as many Cubes as there hear frequency-bands 
	// in they used fft-window... for each, increase its possition-offset
	// by its size alled with the offset-direction
	for(int i = 0; i<SPECTRUM_SIZE;i++)
	{
		offset.x = X+i;
		bands[i]=(new Cubus(color,true,false,offset));
		bands[i]->IsGrounded = true;
		bands[i]->scale(getTransform()->scale);
	}
	
	// Setting the Meters "FallOff"...
	// as lower the value, as slower the v
	// visuals will Fall back to Zero if there's no signal.
	fallOffAmount = 0.05f;
	offset = *Vector3::Zero;

	// this sett's the hight of the analyzers chart.
	// band meshures will multiplied by it.
	this->getTransform()->scale.y = 3;

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
		//geting a more equivalent look then real spectrumdata will...  

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
	if(newScaleY < bands[band]->getTransform()->scale.y)
		bands[band]->getTransform()->scale.y -= fallOffAmount;
	else
		bands[band]->getTransform()->scale.y = newScaleY;
}

void
SpectrumAnalyzer::draw(void)
{
	if(IsVisible)
	{
		if(_CHANGEDposition || _CHANGEDrotation || _CHANGEDscale)
		{
			// Re-Positioning,sizeing and rotating the SubCubes
			// if the whole analyzerobject has been moved,resized 
			// or rotated during last update..  -->> maybe this coul'd be don at LateUpdate better... dont know....
			for(int i=0;i<SPECTRUM_SIZE;i++)
			{
				if(_CHANGEDposition)
					bands[i]->getTransform()->position += offset;
				if(_CHANGEDrotation)
					bands[i]->getTransform()->rotation = getTransform()->rotation;
				if(_CHANGEDscale)
					bands[i]->scale(transform.scale.x,bands[i]->getTransform()->scale.y,transform.scale.z);
				
				//draw each subcube....
				bands[i]->draw();
			}
			_CHANGEDposition = _CHANGEDrotation = _CHANGEDscale = false;
			offset = *Vector3::Zero;
		}
		else // if there are no changes...
		{
			// Draw all subcubes
			for(int i=0;i<SPECTRUM_SIZE;i++)
				bands[i]->draw();
		}
	}
}

Vector3
SpectrumAnalyzer::move(float X,float Y,float Z)
{
	offset += IMeshObject::move(X,Y,Z);
	if(offset!=*Vector3::Zero)
		_CHANGEDposition=true;
	return getTransform()->movement;
}

Vector3
SpectrumAnalyzer::rotate(float X,float Y,float Z)
{
	_CHANGEDrotation=true;
	return IMeshObject::rotate(X,Y,Z);
}

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