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
	data32 coloro;
	coloro.byte[1]=0;
	coloro.byte[2]=255;
	coloro.byte[3]=127;
	coloro.byte[0]=63;


	Vector3 temp = this->getTransform()->position;
	//Cubus* _1Of128; 
	//_1Of128 = CreateCuBus(_1Of128,128);

	for(int i = 0; i<SPECTRUM_SIZE;i++)
	{
		bands[i]=(new Cubus(coloro,true,false));//->move(transform.position.x,transform.position.y,transform.position.z+=i);
		bands[i]->move(temp.x,temp.y,temp.x+i);
	}
	


	UPDATE->SignInForEarlyUpdate(this);
	UPDATE->SignInForUpdate(this);
	UPDATE->SignInForLateUpdate(this);


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
	for(int i=0;i<128;i++)
		bands[i]->scale(Vector3(1,this->fftData[i],1));
}

void
SpectrumAnalyzer::DoLate(void)
{
	
}