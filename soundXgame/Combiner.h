#ifndef _COMBINATOR_
#define _COMBINATOR_

#include "Connectable.h"
//#include "IGObject.h"


class Combiner : public IConnectable 
{
private:
	ConID* ConIDKeyPair[2];
	

public:
	Combiner(void);
	~Combiner(void);
	Combiner* SetConiIDKeyPair(ConID* setter1,ConID* setter2);
	virtual Transform* getSubform(unsigned childIndex);
};


#endif