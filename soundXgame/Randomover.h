#ifndef _RANDOMOVER_H_
#define _RANDOMOVER_H_

#include "connectable.h"
#include "UpdateManager.h"

class Randomover :
	public IConnectable ,public IUpdateble
{
private:
	float x,y,z,iX,iY,iZ;
	float speed;
protected:
	virtual bool Initialize(void);
public:
//	virtual ~Randomover(void);


	virtual void DoUpdate(void);
	
};

#endif