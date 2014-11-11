#ifndef __FOURTAINOBJECT_H__
#define __FOURTAINOBJECT_H__

#include "IGObject.h"



class FourtainObject
	: public IMeshObject
{
private:
	
	float timeSinceStart;
	float power;

public:
	FourtainObject(void);
	~FourtainObject(void);
	void Create(void);
	void Clear(void);
	void Update(void);
};


#endif