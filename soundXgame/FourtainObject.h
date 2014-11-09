#ifndef __FOURTAINOBJECT_H__
#define __FOURTAINOBJECT_H__

#include "IGobject.h"


#pragma once
class FourtainObject
	: public IMeshGobject
{
private:
	
	float timeSinceStart;
	float power;

public:
	FourtainObject(void);
	~FourtainObject(void);
	void Update(void);
};


#endif