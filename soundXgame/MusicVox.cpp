#include "VoxGrid.h"
#include "MusicVox.h"
#include "Connectable.h"
#include "VoxControl.h"

MusicVox::MusicVox(void)
{
	Line[0].MINClampf= 0;
	Line[0].MAXClampf= 100;
	Line[0].threshold= 0.5;
	Line[0].fallOff = 0.001;
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
		for(((VoxControl*)this->Connection()->conXtor)->current=0;((VoxControl*)this->Connection()->conXtor)->current<6;((VoxControl*)this->Connection()->conXtor)->current++)
			((VoxControl*)this->Connection()->conXtor)->vConnection()->BumpFactor = 0.5+(Motivator/500);
		break;
	case 1:
		vec2.y = 1+ vec2.x-Motivator;
		vec2.x = 2+ Motivator;
		if(vec2.x>Motivator && vec2.y<0)
			for(((VoxControl*)this->Connection()->conXtor)->current=0;((VoxControl*)this->Connection()->conXtor)->current<6;((VoxControl*)this->Connection()->conXtor)->current++)
				((VoxControl*)this->Connection()->conXtor)->vConnection()->flip(++((VoxControl*)this->Connection()->conXtor)->vConnection()->flipt);//this->Connection()->getTransform()->up);
		

		for(((VoxControl*)this->Connection()->conXtor)->current=0;((VoxControl*)this->Connection()->conXtor)->current<6;((VoxControl*)this->Connection()->conXtor)->current++)
			((VoxControl*)this->Connection()->conXtor)->vConnection()->MainSizzes = vec2;
		break;
	}
}

float*
MusicVox::GetFFTData(void)
{
	return (float*)AUDIO->GetBackgroundAudioFFT(FFT_SIZE::Small);
}