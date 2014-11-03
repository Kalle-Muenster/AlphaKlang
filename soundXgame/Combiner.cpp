#include "Combiner.h"
#include "DataStructs.h"

Combiner::Combiner(void)
{
	//	ConIDKeyPair[0] = id;
	//	ConIDKeyPair[1] = &connection->conXtor->ConIDs[*ConIDKeyPair[0]];
}


Combiner::~Combiner(void)
{
}


Combiner* 
Combiner::SetConiIDKeyPair(ConID* setter1,ConID* setter2)
{
	ConIDKeyPair[0] = setter1;
	ConIDKeyPair[1] = setter2;
	return this;
}

TransformA*
Combiner::getSubform(unsigned childIndex)
{
	return GetConnected<IConnectable>(*ConIDKeyPair[childIndex])->connection->getTransform();
}