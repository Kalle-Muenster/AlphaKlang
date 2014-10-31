#ifndef __YETI_INCLUDET__
#define __YETI_INCLUDET__

#include "Connectable.h"
#include "InputManager.h"
#include "IGobject.h"

class Yeti : public IGobject, public IInteractive , public IConnectable
{
private:

	

public:
	Yeti(void);
	Yeti(const char* objFile,const char* textureFile,bool addToSceneGraph);
	~Yeti(void);

//	IConnectable* connector;
	void keyPress(char);
	void mouseMotion(int,int);
};

#endif