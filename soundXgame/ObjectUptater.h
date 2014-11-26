#ifndef _OBJECTUPDATER_H_
#define _OBJECTUPDATER_H_

#include "IGobject.h"
#include "UpdateManager.h"
#include "Connectable.h"



class ObjectUpdater :
	public IUpdateble, 
	public IConnectable
{
public:
	typedef void (*UpdateCallback)(void);
	virtual bool Initialize(void);
	virtual ~ObjectUpdater(void);
	virtual void DoEarly(void);
	virtual void DoUpdate(void);
	virtual void DoLate(void);
	void SetUpdateCallback(UPDATE_TIME,UpdateCallback);
private:
	UpdateCallback callbacks[3]; 
};

#endif