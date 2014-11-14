#ifndef _CUBUS_H_
#define _CUBUS_H_

#include "igobject.h"
#include "IMeshObject.h"

class Cubus :
	public IMeshObject
{
public:
	Cubus(void);
	Cubus(string textureFile,bool transparent);
	void InitializeCubus(void);
	void InitializeCubus(string texturefile,bool useTransparenz = false);
	void InitializeCubus(string texturefile,string audioFile,bool useTransparenz=false);
	void SetTexture(string);
	void SetCollisionSound(string);
	virtual ~Cubus(void);
};



#endif