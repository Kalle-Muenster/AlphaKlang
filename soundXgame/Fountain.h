#ifndef __FOUNDTAIN_H___
#define __FOUNDTAIN_H__


#include "FourtainObject.h"
#include "UpdateManager.h"


class Fountain
	: public IUpdateble
{
private:
	int size;
	int spawnLeft;
	std::vector<FourtainObject*> objects;
	double timer;
	void Spawn(void);
	float delay;


public:
	Fountain(void);
	~Fountain(void);
	
	/* Design Pattern -> Object Pool */
	void setMaxSize(int i);
	void createRange(void);
	FourtainObject* getObject(void);
	void release(FourtainObject* obj);
	void Clear(void);

	/* Logic */
	virtual void DoUpdate(void);
	void Draw(void);
};

#endif