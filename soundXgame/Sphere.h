#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "imeshobject.h"


class Sphere :
	public IMeshObject
{
public:
//	Sphere(void);
	Sphere(bool drawBackfaces=false);
	virtual ~Sphere(void);
};

#endif