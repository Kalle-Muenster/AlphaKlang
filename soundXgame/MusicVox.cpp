#include "VoxGrid.h"
#include "MusicVox.h"
#include "Connectable.h"
#include "VoxControl.h"

MusicVox::MusicVox(void)
{
	Line[0].MINClampf= 0;
	Line[0].MAXClampf= 100;
	Line[0].threshold= 0.05;
	Line[0].fallOff= 0.00001;
	sensitivity = 100;
}

bool
MusicVox::Initialize(void)
{
	vec2 = *VectorF::Zero;

	if(this->Connection()->HasConnected<VoxControl>())
		this->VoxcontrollID = this->Connection()->conXtor->GetConnectionID<VoxControl>();
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
		((VoxGrid*)this->Connection())->BumpFactor = 0.3+(Motivator/500);
		break;
	case 1:
		
		if(vec2.x>Motivator && vec2.y<0)((VoxGrid*)this->Connection())->flip(++((VoxGrid*)this->Connection())->flipt);//this->Connection()->getTransform()->up);
		vec2.y = 1+ vec2.x-Motivator;
		vec2.x = 1.5+ Motivator;
		((VoxGrid*)this->Connection())->MainSizzes = vec2;
		break;
	}
}

float*
MusicVox::GetFFTData(void)
{
	return (float*)AUDIO->GetBackgroundAudioFFT(FFT_SIZE::Small);
}