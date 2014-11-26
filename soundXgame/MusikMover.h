#ifndef _MUSICMOVER_H_
#define _MUSIKMOVER_H_

#include "imusiclistener.h"
#include "ObjectUptater.h"
#include "UpdateManager.h"

class MusikMover :
	public IMusicListener,
	public IConnectable
{
public:
	MusikMover(void);

	}

};

#endif