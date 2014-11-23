#ifndef _PERCEPTOR_H_
#define _PERCEPTOR_H_

#include "connectable.h"
#include "UpdateManager.h"

class Perceptor :
	public IConnectable,
	public IUpdateble
{
private:
	float radius;
	int   mode;
public:
	Perceptor(void);
	virtual ~Perceptor(void);
};

#endif