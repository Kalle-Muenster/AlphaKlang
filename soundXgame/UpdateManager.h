#include <vector>

#ifndef _UPDATEMANAGER_H_
#define _UPDATEMANAGER_H_

#define MAXIMUM_NUMBER_OF_UPDATECLIENTS (1000)

typedef unsigned UpdateID;

class IUpdateble
{
public:
	UpdateID UpdID;
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
	
	static void SignInForEarlyUpdate(IUpdateble*);
	static void SignInForUpdate(IUpdateble*);
	static void SignInForLateUpdate(IUpdateble*);
	
	static void SignOutForUpdate(IUpdateble*);
	static void SignOutForEarlyUpdate(IUpdateble*);
	static void SignOutForLateUpdate(IUpdateble*);

	static void DoAllTheUpdates(void);

};

#endif