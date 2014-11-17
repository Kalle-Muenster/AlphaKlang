#include <stdio.h>
#include <vector>
#include "UpdateManager.h"

UpdateManager* instance;

//IUpdateble* earlyUpdates[MAXIMUM_NUMBER_OF_UPDATECLIENTS/4];
//IUpdateble* updates[MAXIMUM_NUMBER_OF_UPDATECLIENTS/2];
//IUpdateble* lateUpdates[MAXIMUM_NUMBER_OF_UPDATECLIENTS/4];

//unsigned NumberOfClients = 0;
//unsigned NumberOfLateClients = 0;
//unsigned NumberOfEarlyClients = 0;

static int updateCounter = 0;
static int updateEarlyCounter = 0;
static int updateLateCounter = 0;

static std::vector<IUpdateble*> updates;
static std::vector<IUpdateble*> earlyUpdates;
static std::vector<IUpdateble*> lateUpdates;

UpdateManager::UpdateManager(void)
{
}

UpdateManager::~UpdateManager(void)
{
}

UpdateManager*
UpdateManager::getInstance(void)
{
	if(!instance)
		instance = new UpdateManager();
	return instance;
}

void
UpdateManager::SignInForUpdate(IUpdateble* updatebleInstance)
{
	updates.push_back(updatebleInstance);
	updatebleInstance->UpdID = ++updateCounter;
}

void
UpdateManager::SignInForEarlyUpdate(IUpdateble* updatebleInstance)
{
	earlyUpdates.push_back(updatebleInstance);
	updatebleInstance->UpdID = ++updateEarlyCounter;
}

void
UpdateManager::SignInForLateUpdate(IUpdateble* updatebleInstance)
{
	lateUpdates.push_back(updatebleInstance);
	updatebleInstance->UpdID = ++updateLateCounter;
}

void
UpdateManager::SignOutForUpdate(IUpdateble* updatebleInstance)
{
	updates.erase(updates.begin() + updatebleInstance->UpdID); // -1 ??
}

void
UpdateManager::SignOutForEarlyUpdate(IUpdateble* updatebleInstance)
{
	earlyUpdates.erase(earlyUpdates.begin() + updatebleInstance->UpdID);
}

void
UpdateManager::SignOutForLateUpdate(IUpdateble* updatebleInstance)
{
	lateUpdates.erase(lateUpdates.begin() + updatebleInstance->UpdID);
}

void
UpdateManager::DoAllTheUpdates(void)
{
	for (unsigned i=0; i < earlyUpdates.size(); ++i)
		earlyUpdates[i]->DoEarly();

	for (unsigned i=0; i < updates.size(); ++i)
		updates[i]->DoUpdate();
	
	for (unsigned i=0; i < lateUpdates.size(); ++i)
		lateUpdates[i]->DoLate();
}

IUpdateble::IUpdateble(void)
{
	this->InitiateUpdatable();
}

IUpdateble::~IUpdateble(void)
{

}

void
IUpdateble::InitiateUpdatable(void)
{
	UpdateManager::SignInForUpdate(this);
}
