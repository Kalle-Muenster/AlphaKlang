#ifndef __FOUNDTAIN_H___
#define __FOUNDTAIN_H__


#include "FountainObject.h"
#include "UpdateManager.h"
#include "Transform.h"


class Fountain
	: public IUpdateble
{
private:
	double timer, timer2;
	void Spawn(void);
	float spawnDelay;
	float deleteDelay;
	Transform transform;

	/* Object Pool */
	int size;
	std::vector<FountainObject*> objects;
	std::vector<FountainObject*> spawnedObjects;

public:
	//enum { SIZE = 100 };

	Fountain(void);
	~Fountain(void);
	
	/* Object Pool */
	void setMaxSize(int i);
	void createRange(void);
	FountainObject* getObject(void);
	void release(FountainObject* obj);
	void Clear(void);

	/* Logic */
	virtual void DoUpdate(void);
	void Draw(void);
};

#endif