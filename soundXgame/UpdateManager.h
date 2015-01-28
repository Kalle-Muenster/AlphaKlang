#include <vector>

#ifndef _UPDATEMANAGER_H_
#define _UPDATEMANAGER_H_

#define MAXIMUM_NUMBER_OF_UPDATECLIENTS (1000)
#define UPDATE UpdateManager::getInstance()


enum UPdTIME
{EARLY=0,MAIN=1,LATE=2};

struct UpdateID
{
	unsigned Main,Early,Late;
	unsigned& operator[](UPdTIME); 
};

class IUpdateble
{
public:
	UpdateID UpdID;
	bool IsUpdatingActive;
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
	static const char* start(void);
	static UpdateManager* getInstance(void);
	
	void SignInForEarlyUpdate(IUpdateble*);
	void SignInForUpdate(IUpdateble*);
	void SignInForLateUpdate(IUpdateble*);
	
	void SignOutFromUpdate(IUpdateble*);
	void SignOutFromEarlyUpdate(IUpdateble*);
	void SignOutFromLateUpdate(IUpdateble*);

	void DoTheUpdates(void);
	void DoTheLateUpdates(void);

};

#endif