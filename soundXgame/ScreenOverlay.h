#ifndef _SCREENOVERLAY_2D_
#define _SCREENOVERLAY_2D_

#include "IMeshobject.h"


class ScreenOverlay :
	public IGObject
{
public:
	Texture texture;
	ScreenOverlay(void);
	virtual void Initialize(char* fileName);
	virtual ~ScreenOverlay(void);
	void LoadeImage(char* filename);
	virtual void draw(void);
	VectorF scalefactor;
	virtual Vector3 scale(float,float,float);
};

#endif