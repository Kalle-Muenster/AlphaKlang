#ifndef _CUBUS_H_
#define _CUBUS_H_

#include "igobject.h"


class Cubus :
	public IMeshGobject
{
public:
	Cubus(void);
	void InitializeCubus(string meshfile,string texturefile);
	virtual ~Cubus(void);
};

#endif