#ifndef _MUSIKUBUS_
#define _MUSIKUBUS_

#include "cubus.h"
#include "MusicInteractor.h"

class Musicubus :
	public Cubus
{
private:

	MusicInteractor* musikInput;

public:
	Musicubus(void);
	Musicubus(string textureFile,bool drawBackfaces=false,bool addToScene = true);
	virtual ~Musicubus(void);
};



#endif