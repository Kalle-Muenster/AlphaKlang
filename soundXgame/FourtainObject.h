#ifndef __FOURTAINOBJECT_H__
#define __FOURTAINOBJECT_H__

#include "IGObject.h"
#include "IMeshObject.h"
#include "UpdateManager.h"



class FourtainObject
	: public IMeshObject, public IUpdateble
{
private:
	
	//float timeSinceStart;
public:
	float power;
	float gravity;
	float speed;
	float expanding; // Power for Directionvector (wie stark ausweiten)
	Vector3 direction; // Directionvector to Expand

	FourtainObject(void);
	~FourtainObject(void);
	void AddToScene(void);
	void Clear(void);
	//void Update(void);
	virtual void DoUpdate(void);
};


#endif