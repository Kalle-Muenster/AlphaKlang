#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "imeshobject.h"


class Sphere :
	public IMeshObject
{
public:
	Sphere(string mesh,string tex,bool backFaces = false);
	Sphere(bool drawBackfaces=false);
	virtual ~Sphere(void);

};

#endif