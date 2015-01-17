#ifndef _CUBUS_H_
#define _CUBUS_H_

#include "igobject.h"
#include "IMeshObject.h"





class Cubus :
	public IMeshObject
{
protected:
	Cubus(void);
public:
	
	Cubus(bool initializeed);
	Cubus(data32 color,bool backfaces=false,bool addToScene = true,Vector3 positionOffset = *Vector3::Zero);
	Cubus(string textureFile,bool drawBackfaces=false,bool addToScene = true,Vector3 positionOffset = *Vector3::Zero);
	//void InitializeCubus(void);
	void InitializeCubus(bool addToScene = true,Vector3 positionOffset = *Vector3::Zero);
	void InitializeCubus( string texturefile,bool drawBackfaces=false,bool addToScene = true);
	void InitializeCubus( string texturefile,string audioFile,bool drawBackfaces=false,bool addToScene = true);
	void SetTexture(string);
	void SetCollisionSound(string);
	virtual ~Cubus(void);
	Cubus* CreateCuBus(IGObject* parent,int howmany);
	
};



#endif