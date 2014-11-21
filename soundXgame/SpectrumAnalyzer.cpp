#include "SpectrumAnalyzer.h"
#include "projectMacros.h"
#include "Connectable.h"

SpectrumAnalyzer::SpectrumAnalyzer(void)
{
	IGObject::InitializeObject();
	InitializeSpectrumAnalyzer();
}


SpectrumAnalyzer::~SpectrumAnalyzer(void)
{
	UPDATE->SignOutFromEarlyUpdate(this);
	UPDATE->SignOutFromUpdate(this);
	UPDATE->SignOutFromLateUpdate(this);
	delete[] bands;
}

void
SpectrumAnalyzer::InitializeSpectrumAnalyzer(void)
{
	color.byte[1]=0;
	color.byte[2]=255;
	color.byte[3]=127;
	color.byte[0]=10;


	offset = this->getTransform()->position;
	offset.y += 0.5;
	float X = offset.x;
	X-=(SPECTRUM_SIZE*0.5);

	this->getTransform()->scale.x=0.1;
	this->getTransform()->scale.y=0.1;
	this->getTransform()->scale.z=2;

	for(int i = 0; i<SPECTRUM_SIZE;i++)
	{
		offset.x = X+i;
		bands[i]=(new Cubus(color,true,false,offset));
		bands[i]->scale(getTransform()->scale);
	}
	
	fallOffAmount = 0.05f;
	offset = *Vector3::Zero;
	this->getTransform()->scale.y = 3;

	UPDATE->SignInForEarlyUpdate(this);
//	UPDATE->SignInForLateUpdate(this);
}

void
SpectrumAnalyzer::DoEarly(void)
{
	this->fftData = (float*)AUDIO->GetBackgroundAudioFFT(FFT_SIZE::Small);
}

void
SpectrumAnalyzer::DoUpdate(void)
{
	float changeFactor;
	for(int i=0;i<SPECTRUM_SIZE;i++)
	{
	//	changeFactor = (this->fftData[i]*((float)i*0.1));
		changeFactor = (this->fftData[i]*((float)1+i*0.128));
		ChangeSize(i,changeFactor * this->getTransform()->scale.y);
		ChangeColor(i,changeFactor);
	}
}

void 
SpectrumAnalyzer::ChangeColor(int index,float factor)
{
	color.byte[1] = ((factor*10) * 255.0);
	color.byte[2] = (1.0 - (factor*10))*255.0;
	bands[index]->SetColor(color);
}
void
SpectrumAnalyzer::ChangeSize(int band,float newScaleY)
{
	if(newScaleY < bands[band]->getTransform()->scale.y)
		bands[band]->getTransform()->scale.y -= fallOffAmount;
	else
		bands[band]->getTransform()->scale.y = newScaleY;
}


void
SpectrumAnalyzer::DoLate(void)
{

}

void
SpectrumAnalyzer::draw(void)
{
	if(IsVisible)
	{
		if(_CHANGEDposition || _CHANGEDrotation || _CHANGEDscale)
		{	
			for(int i=0;i<SPECTRUM_SIZE;i++)
			{
				if(_CHANGEDposition)
					bands[i]->getTransform()->position += offset;
				if(_CHANGEDrotation)
					bands[i]->getTransform()->rotation = getTransform()->rotation;
				if(_CHANGEDscale)
					bands[i]->scale(transform.scale.x,bands[i]->getTransform()->scale.y,transform.scale.z);

				bands[i]->draw();
			}
			_CHANGEDposition = _CHANGEDrotation = _CHANGEDscale = false;
			offset = *Vector3::Zero;
		}
		else
		{
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