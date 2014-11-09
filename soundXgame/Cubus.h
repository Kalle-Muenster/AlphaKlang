#ifndef _CUBUS_H_
#define _CUBUS_H_

#include "igobject.h"


class Cubus :
	public IMeshGobject
{
public:
	Cubus(void);
	void InitializeCubus(void);
	void InitializeCubus(string texturefile);
	void InitializeCubus(string texturefile,string audioFile);
	void SetTexture(string);
	void SetCollisionSound(string);
	virtual ~Cubus(void);
};



#endif