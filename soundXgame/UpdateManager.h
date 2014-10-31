#ifndef _UPDATEMANAGER_H_
#define _UPDATEMANAGER_H_

#define MAXIMUM_NUMBER_OF_UPDATECLIENTS (1000)

typedef unsigned UpdateID;

class IUpdateble
{
protected:
	UpdateID UpdID;
public:
	IUpdateble(void);
	virtual void InitiateUpdatable(void);
	virtual ~IUpdateble(void);
	virtual void DoUpdate(void)=0;
	virtual void DoEarly(void){};
	virtual void DoLate(void){};
};

class UpdateManager
{
public:
	UpdateManager(void);
	virtual ~UpdateManager(void);
	static UpdateManager* getInstance(void);
	static UpdateID SignInForEarlyUpdate(IUpdateble*);
	static UpdateID SignInForUpdate(IUpdateble*);
	static UpdateID SignInForLateUpdate(IUpdateble*);
	static void DoAllTheUpdates(void);
};

#endif