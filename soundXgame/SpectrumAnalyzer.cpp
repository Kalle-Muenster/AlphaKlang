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


	Vector3 temp = this->getTransform()->position;
	//Cubus* _1Of128; 
	//_1Of128 = CreateCuBus(_1Of128,128);

	for(int i = 0; i<SPECTRUM_SIZE;i++)
	{
		bands[i]=(new Cubus(color,true,false));//->move(transform.position.x,transform.position.y,transform.position.z+=i);
		bands[i]->move(temp.x + i * 0.1f,temp.y,temp.z);
	}
	


	UPDATE->SignInForEarlyUpdate(this);
	UPDATE->SignInForUpdate(this);
	UPDATE->SignInForLateUpdate(this);
	this->getTransform()->scale.y=10;

}

void
SpectrumAnalyzer::DoEarly(void)
{
	this->fftData = (float*)AUDIO->GetBackgroundAudioFFT((FFT_SIZE)SPECTRUM_SIZE);
	_gotThisFrameSpectrumData=true;
}

void
SpectrumAnalyzer::DoUpdate(void)
{
	Vector3 sizer = getTransform()->scale;
	sizer.x = 0.1;
	sizer.z = 0.25;
	for(int i=0;i<128;i++)
	{
		float changeFactor = (this->fftData[i]*((float)i*0.1));
		sizer.y = changeFactor * this->getTransform()->scale.y;
		bands[i]->scale(sizer);
		ChangeColor(changeFactor);
		bands[i]->SetColor(color);
	}
}

void 
SpectrumAnalyzer::ChangeColor(float bandLevel)
{
	color.byte[1] = ((bandLevel*10) * 255.0);
	color.byte[2] = (1.0 - (bandLevel*10))*255.0;
}
void
SpectrumAnalyzer::ChangeSize(float bandLevel)
{

}

void
SpectrumAnalyzer::DoLate(void)
{
	
}

void
SpectrumAnalyzer::draw(void)
{

}