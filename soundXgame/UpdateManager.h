#include <vector>

#ifndef _UPDATEMANAGER_H_
#define _UPDATEMANAGER_H_

#define MAXIMUM_NUMBER_OF_UPDATECLIENTS (1000)

typedef unsigned UpID;
struct UpdateID
{
	unsigned Main,Early,Late;
};

class IUpdateble
{
public:
	enum UPDATE_TIME
	{EARLY=0,MAIN=1,LATE=2};
	UpdateID UpdID;
	IUpdateble(void);
	virtual void InitiateUpdatable(void);
	virtual ~IUpdateble(void);
	virtual void DoUpdate(void){};
	virtual void DoEarly(void){};
	virtual void DoLate(void){};
};

class UpdateManager
{
public:

	UpdateManager(void);
	virtual ~UpdateManager(void);
	static UpdateManager* getInstance(void);
	
	void SignInForEarlyUpdate(IUpdateble*);
	void SignInForUpdate(IUpdateble*);
	void SignInForLateUpdate(IUpdateble*);
	
	void SignOutFromUpdate(IUpdateble*);
	void SignOutFromEarlyUpdate(IUpdateble*);
	void SignOutFromLateUpdate(IUpdateble*);

	void DoAllTheUpdates(void);

};

#endif