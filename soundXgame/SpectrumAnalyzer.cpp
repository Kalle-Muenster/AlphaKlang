#include "SpectrumAnalyzer.h"
#include "projectMacros.h"
#include "Connectable.h"

SpectrumAnalyzer::SpectrumAnalyzer(void)
{
	IGObject::InitializeObject();
	InitializeSpectrumAnalyzer();
	_gotThisFrameSpectrumData = false;
}


SpectrumAnalyzer::~SpectrumAnalyzer(void)
{
	UPDATE->SignOutFromEarlyUpdate(this);
	UPDATE->SignOutFromUpdate(this);
	UPDATE->SignOutFromLateUpdate(this);
}

void
SpectrumAnalyzer::InitializeSpectrumAnalyzer(void)
{

	color.byte[1]=0;
	color.byte[2]=255;
	color.byte[3]=127;
	color.byte[0]=10;


	Vector3 offset = this->getTransform()->position;
	offset.y += 0.5;
	float X = offset.x;
	X-=63;

	for(int i = 0; i<SPECTRUM_SIZE;i++)
	{
		offset.x = X+i;
		bands[i]=(new Cubus(color,true,false,offset));
	}
	
	fallOffAmount = 0.05f;

	UPDATE->SignInForEarlyUpdate(this);
	UPDATE->SignInForUpdate(this);
	UPDATE->SignInForLateUpdate(this);
	this->getTransform()->scale.x=0.1;
	this->getTransform()->scale.y=10;
	this->getTransform()->scale.z=0.25;
}

void
SpectrumAnalyzer::DoEarly(void)
{
	this->fftData = (float*)AUDIO->GetBackgroundAudioFFT(FFT_SIZE::Small);
	_gotThisFrameSpectrumData=true;
}

void
SpectrumAnalyzer::DoUpdate(void)
{
	Vector3 sizer = getTransform()->scale;
	for(int i=0;i<SPECTRUM_SIZE;i++)
	{
		float changeFactor = (this->fftData[i]*((float)i*0.1));
		sizer.y = changeFactor * this->getTransform()->scale.y;
		ChangeSize(i,sizer);
		ChangeColor(i,changeFactor);
	}
}

void 
SpectrumAnalyzer::ChangeColor(int index,float bandLevel)
{
	color.byte[1] = ((bandLevel*10) * 255.0);
	color.byte[2] = (1.0 - (bandLevel*10))*255.0;
	bands[index]->SetColor(color);
}
void
SpectrumAnalyzer::ChangeSize(int band,Vector3 newScale)
{
	if(bands[band]->getTransform()->scale != newScale)
	{
		if(bands[band]->getTransform()->scale.y > newScale.y)
			newScale.y = bands[band]->getTransform()->scale.y - fallOffAmount;
		bands[band]->scale(newScale);
	}
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
		for(int i=0;i<SPECTRUM_SIZE;i++)
		bands[i]->draw();
	}
}

Vector3
SpectrumAnalyzer::move(float X,float Y,float Z)
{
	IMeshObject::move(X,Y,Z);
	for(int i = 0; i<SPECTRUM_SIZE;i++)
		bands[i]->getTransform()->position += getTransform()->movement;
	return getTransform()->movement;
}

Vector3
SpectrumAnalyzer::rotate(float X,float Y,float Z)
{
	IMeshObject::rotate(X,Y,Z);
	for(int i=0;i<SPECTRUM_SIZE;i++)
		bands[i]->getTransform()->rotation = getTransform()->rotation;
	return getTransform()->rotation;
}

Vector3
SpectrumAnalyzer::scale(float X,float Y,float Z)
{
	IMeshObject::scale(X,Y,Z);
	for(int i=0;i<SPECTRUM_SIZE;i++)
		bands[i]->scale(X,Y,Z);
	return getTransform()->scale;
}