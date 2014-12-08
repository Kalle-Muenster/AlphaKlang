#ifndef _CMUSICVOX_
#define _CMUSICVOX_

#include "musiclistener.h"

class MusicVox :
	public MusicListener,
	public IConnectable
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