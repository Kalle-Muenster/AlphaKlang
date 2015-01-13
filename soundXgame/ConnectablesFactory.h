#ifndef _CONNECTABLES°FAKTORY_
#define _CONNECTABLES°FAKTORY_

#include "Connectable.h"

template<typename NonConnectableType> 
class Connectable : public NonConnectableType ,  public IConnectable 
{
public:
	virtual bool Initialize(void)
	{
		TypeHashCode = typeid(this).hash_code();
		return true;
	}
};

#endif
