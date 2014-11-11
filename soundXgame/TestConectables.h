#ifndef _TESTCONNECTABLES_H_
#define _TESTCONNECTABLES_H_

#include "observer.h"
#include "Cam.h"

class YetiInteractive :
	public CInteractive
{
public:
	YetiInteractive(void){}
	virtual YetiInteractive::~YetiInteractive(void){}

	virtual void keyPress(char key);
//	virtual void mouseMotion(int x, int y);

};



#endif