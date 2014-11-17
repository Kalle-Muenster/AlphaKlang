#ifndef _CUBUS_H_
#define _CUBUS_H_

#include "igobject.h"
#include "IMeshObject.h"

class Cubus :
	public IMeshObject
{
public:
	Cubus(void);
	Cubus(data32 color,bool backfaces=false);
	Cubus(string textureFile,bool drawBackfaces=false);
	void InitializeCubus(void);
	void InitializeCubus(string texturefile,bool drawBackfaces=false);
	void InitializeCubus(string texturefile,string audioFile,bool drawBackfaces=false);
	void SetTexture(string);
	void SetCollisionSound(string);
	virtual ~Cubus(void);
};



#endif