
#include "ObjectPool.h"



ObjectPool* objectPool;

unsigned GObjectIDs[AMOUNT_ON_GOBJECTS];
IGobject gobjects[AMOUNT_ON_GOBJECTS];
unsigned NumberOfGobjectsInVallet;

unsigned doubleIDs[INITALAMOUNT_ON_DOUBLES];
double Doubles[INITALAMOUNT_ON_DOUBLES];
unsigned NumberOfDoublesInVallet;



ObjectPool::ObjectPool(void)
{
	NumberOfDoublesInVallet=0;
	for(int i=0;i<INITALAMOUNT_ON_DOUBLES;i++)
	{
		Doubles[i]=0.0;
		doubleIDs[i]=0;
	}
}


ObjectPool::~ObjectPool(void)
{
	
		
}

ObjectPool*
ObjectPool::GetInstance(void)
{
	if(!objectPool)
		objectPool= new ObjectPool();

	return objectPool;
}

double*
ObjectPool::GetADouble(unsigned & ID)
{
	for(int i=0;i<INITALAMOUNT_ON_DOUBLES;i++)
		if(doubleIDs[i]==0)
		{
			doubleIDs[i]=ID;
			NumberOfDoublesInVallet++;
			return &Doubles[i];
		}
	throw "Nö!";
}

void
ObjectPool::RejectDouble(double* D,unsigned& ID)
{
	delete D;

		for(int i=0;i<INITALAMOUNT_ON_DOUBLES;i++)
			if(doubleIDs[i]==ID)
			{
				doubleIDs[i]=0;
				Doubles[i]=0.0;
			}
}

IvOxject*
ObjectPool::GetAVoxject(unsigned& ID)
{
	return NULL;
}
	
void
ObjectPool::RejectVoxject(IvOxject* gobj,unsigned& ID)
{

}