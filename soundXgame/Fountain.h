#ifndef __FOUNDTAIN_H___
#define __FOUNDTAIN_H__


#include "FourtainObject.h"
#include "UpdateManager.h"

#pragma once
class Fountain : public IUpdateble
{
private:
	int size;
	std::vector<FourtainObject*> objects;

	FourtainObject* cube;

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
	void Init(void);
	virtual void DoUpdate(void);
	void Draw(void);
};

#endif