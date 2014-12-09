#ifndef __FOUNDTAIN_H___
#define __FOUNDTAIN_H__


#include "FountainObject.h"
#include "UpdateManager.h"
#include "Transform.h"
#include "Ground.h"
#include "MusicListener.h"

class Fountain :
	public IUpdateble,
	 public MusicListener
{
private:
	double timer, timer2;
	void Spawn(void);
	float spawnDelay;
	float deleteDelay;
	Transform transform;

	/* Object Pool */
	std::vector<FountainObject*> objects;
	std::vector<FountainObject*> spawnedObjects;

public:
	int size;
	//enum { SIZE = 100 };

	Fountain(void);
	~Fountain(void);
	void SetPosition(Vector3 pos);
	
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