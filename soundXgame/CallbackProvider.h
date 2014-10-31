#ifndef _ICcallbackProvider_
#define _ICcallbackProvider_

#include "connectable.h"
#include "IMusicListener.h"

class CallbackProvider :
	public IConnectable
{
protected:

public:
	CallbackProvider(void);
	virtual ~CallbackProvider(void);

};

#endif