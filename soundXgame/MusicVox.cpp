#include "VoxGrid.h"
#include "MusicVox.h"
#include "Connectable.h"
#include "VoxControl.h"

MusicVox::MusicVox(void)
{
	Line[0].MINClampf= 0;
	Line[0].MAXClampf= 100;
	Line[0].threshold= 0.5f;
	Line[0].fallOff = 0.001;
	Line[0].lowerAmp = 2;
	Line[0].upperAmp = 4;
	Line[0].Effect.ControllerActive = false;

	sensitivity = 100;
}

bool
MusicVox::Initialize(void)
{
	vec2 = *VectorF::Zero;
	return true;
}

MusicVox::~MusicVox(void)
{

}

void
MusicVox::MotivatorFunction(float Motivator,int number)
{
	switch(number)
	{
	case 0:
		for(this->Connection()->conXtor->current=0;this->Connection()->conXtor->current<6;this->Connection()->conXtor->current++)
			((VoxControl*)this->Connection()->conXtor)->vConnection()->BumpFactor = 0.5+(Motivator/100);
		break;

	case 1:
		if(vec2.x>Motivator-2 && vec2.y<vec2.x+1+Motivator)
			for(this->Connection()->conXtor->current=0;this->Connection()->conXtor->current<6;this->Connection()->conXtor->current++)
				((VoxControl*)this->Connection()->conXtor)->vConnection()->flip(++((VoxControl*)this->Connection()->conXtor)->vConnection()->flipt);//this->Connection()->getTransform()->up);
		
		vec2.y = 1+ vec2.x-Motivator;
		vec2.x = 2+ Motivator;
		for(this->Connection()->conXtor->current=0;this->Connection()->conXtor->current<6;this->Connection()->conXtor->current++)
			((VoxControl*)this->Connection()->conXtor)->vConnection()->MainSizzes = vec2;
		break;
	}
}

float*
MusicVox::GetFFTData(void)
{
	return (float*)AUDIO->GetBackgroundAudioFFT(FFT_SIZE::Small);
}