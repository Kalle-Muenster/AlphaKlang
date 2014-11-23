#ifndef _COLLIDER_H_
#define _COLLIDER_H_

#include "connectable.h"


class Collider :
	public IConnectable
{
public:
	Collider(void);
	virtual ~Collider(void);
};

#endif