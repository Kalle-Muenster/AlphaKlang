#include "UpdateManager.h"

UpdateManager* updata;

IUpdateble* earlyUpdates[MAXIMUM_NUMBER_OF_UPDATECLIENTS/4];
IUpdateble* updates[MAXIMUM_NUMBER_OF_UPDATECLIENTS/2];
IUpdateble* lateUpdates[MAXIMUM_NUMBER_OF_UPDATECLIENTS/4];

unsigned NumberOfClients = 0;
unsigned NumberOfLateClients = 0;
unsigned NumberOfEarlyClients = 0;

UpdateManager::UpdateManager(void)
{
}


UpdateManager::~UpdateManager(void)
{
}

UpdateManager*
UpdateManager::getInstance(void)
{
	if(!updata)
		updata = new UpdateManager();

	return updata;
}

UpdateID
UpdateManager::SignInForUpdate(IUpdateble* updatebleInstance)
{
	updates[NumberOfClients] = updatebleInstance;
	return ++NumberOfClients;
}

UpdateID
UpdateManager::SignInForEarlyUpdate(IUpdateble* updatebleInstance)
{
	updates[NumberOfEarlyClients] = updatebleInstance;
	return ++NumberOfEarlyClients;
}

UpdateID
UpdateManager::SignInForLateUpdate(IUpdateble* updatebleInstance)
{
	lateUpdates[NumberOfLateClients] = updatebleInstance;
	return ++NumberOfLateClients;
}

void
UpdateManager::DoAllTheUpdates(void)
{
	unsigned i = 0;
	for(;i<NumberOfEarlyClients;i++)
		earlyUpdates[i]->DoEarly();
	for(i=0;i<NumberOfClients;i++)
		updates[i]->DoUpdate();
	for(i=0;i<NumberOfLateClients;i++)
		lateUpdates[i]->DoLate();
}

IUpdateble::IUpdateble(void)
{
	this->InitiateUpdatable();
}

void
IUpdateble::InitiateUpdatable(void)
{
	UpdateManager::SignInForUpdate(this);
}

IUpdateble::~IUpdateble(void)
{

}