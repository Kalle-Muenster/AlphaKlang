#ifndef _OBJECTFollower_H_
#define _OBJECTFollower_H_

#include "Connectable.h"
#include "UpdateManager.h"

class ObjectFollower :
	public IConnectable,
	public IUpdateble
{
protected:
	Vector3* target;
	Vector3* offset;

public:
	ObjectFollower(void);
	virtual bool Initialize(void);
	virtual void SetTarget(IObjection<IConnectable>*);
	virtual void SetTarget(Transform*);
	virtual void SetTarget(Vector3&);
	virtual void SetOffSet(Vector3&);
	virtual void DoUpdate(void);
	bool OffsetAsScale;
};

#endif