#ifndef __FountainObject_H__
#define __FountainObject_H__

#include "IGObject.h"
#include "IMeshObject.h"
#include "UpdateManager.h"
#include "Randomover.h"



class FountainObject
	: public IMeshObject, public IUpdateble
{
private:
	void Release(void);
public:
	float power;
	float gravity;
	float speed;
	float expanding; // Power for Directionvector (wie stark ausweiten), 1=low,10=high
	float reduction; // Wie schnell der speed entgegen der Schwerkraft aufgeholben werden soll, -2.5=slow, +2.5=fast
	Vector3 direction; // Directionvector to Expand

	FountainObject(void);
	~FountainObject(void);
	void Initialize(void);
	//void AddToScene(void);
	void Clear(void);
	//void Update(void);
	virtual void DoUpdate(void);
};


#endif