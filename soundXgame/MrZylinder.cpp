#include "MrZylinder.h"
#include "AudioObject.h"

MrZylinder::MrZylinder(void)
{
	 InitializeObject("cylinder_tris.obi",true);
//	 GetConnected<AudioEmitter>()->LoadeSample("02.wav");
	 this->color.byte[1] = 128;
	 this->color.byte[2] = 128,
	 this->color.byte[3] = 128,
	 this->color.byte[0] = 255;
}


MrZylinder::~MrZylinder(void)
{
}
