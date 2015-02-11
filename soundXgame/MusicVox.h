#ifndef _CMUSICVOX_
#define _CMUSICVOX_

#include "musiccontroller.h"
#include "SkyBox.h"

class MusicVox :
	public MusicController
{
private:
	ConID VoxcontrollID;
	VectorF vec2;
	Vector3 vec3;
public:
	MusicVox(void);
	virtual bool Initialize(void);
	virtual ~MusicVox(void);
	virtual void MotivatorFunction(float Motivator,int number);
	virtual float* GetFFTData(void);
};

#endif