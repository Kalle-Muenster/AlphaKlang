#include <stdio.h>
#include <vector>
//#include "UpdateManager.h"
#include "projectMacros.h"



IUpdateble* _earlyUpdates[MAXIMUM_NUMBER_OF_UPDATECLIENTS/4];
IUpdateble* _updates[MAXIMUM_NUMBER_OF_UPDATECLIENTS/2];
IUpdateble* _lateUpdates[MAXIMUM_NUMBER_OF_UPDATECLIENTS/4];

unsigned _NumberOfUpdateClients = 0;
unsigned _NumberOfLateClients = 0;
unsigned _NumberOfEarlyClients = 0;

//Constructor
UpdateManager::UpdateManager(void)
{
	_NumberOfEarlyClients =_NumberOfUpdateClients =_NumberOfLateClients = 0;
	int i;
	int MAX=MAXIMUM_NUMBER_OF_UPDATECLIENTS/4;
	for(i=0;i<MAX;i++)
	{
		_earlyUpdates[i] = NULL;
		_updates[i]=NULL;
		_lateUpdates[i]=NULL;
	}
	MAX = MAXIMUM_NUMBER_OF_UPDATECLIENTS/2;
	for(i;i<MAX;i++)
		_updates[i]=NULL;
}


//Destuctor...
UpdateManager::~UpdateManager(void)
{
	delete[] _earlyUpdates;
	delete[] _updates;
	delete[] _lateUpdates;
}


// Signing function...
void // take's a pointer to an Invokationlist and signs in the given client to it..
_SignIn( // before it will check if the client already is in there...
		 IUpdateble** InvokationList, 
		 IUpdateble* client, 
		 unsigned &CurrentAmountOnClients, 
		 unsigned MaximumNumberOfClients 
		 ){
	int FirstEmptySlotFound = -1;
	unsigned counter = 0;
	unsigned index = 0;

	while(counter<CurrentAmountOnClients)
	{
		if(InvokationList[index] == NULL)
			FirstEmptySlotFound = FirstEmptySlotFound<0? index : FirstEmptySlotFound;
		else if(InvokationList[index] != client)
			counter++;
		else 
			return;
		index++;
	}

	if(FirstEmptySlotFound>=0)
	{
		InvokationList[FirstEmptySlotFound]=client;
		InvokationList[FirstEmptySlotFound]->UpdID.Main = FirstEmptySlotFound;
		CurrentAmountOnClients++; 
	}
	else for(index = CurrentAmountOnClients; index < MaximumNumberOfClients ;index++)
	{
		if(InvokationList[index]==NULL)
		{	
			InvokationList[index]=client;
			InvokationList[index]->UpdID.Main=index;
			CurrentAmountOnClients++; 
			return;
		}
	}
}


/* UpdateManager-SignIn's:...  */
void
UpdateManager::SignInForUpdate(IUpdateble* updatebleInstance)
{
	_SignIn(&_updates[0], updatebleInstance, _NumberOfUpdateClients, MAXIMUM_NUMBER_OF_UPDATECLIENTS/2);
}

void
UpdateManager::SignInForEarlyUpdate(IUpdateble* updatebleInstance)
{
	_SignIn(&_earlyUpdates[0], updatebleInstance, _NumberOfEarlyClients, MAXIMUM_NUMBER_OF_UPDATECLIENTS/4);
}

void
UpdateManager::SignInForLateUpdate(IUpdateble* updatebleInstance)
{
	_SignIn(&_lateUpdates[0], updatebleInstance, _NumberOfLateClients, MAXIMUM_NUMBER_OF_UPDATECLIENTS/4);
}

/* SignOut's:.... */
void
UpdateManager::SignOutFromUpdate(IUpdateble* updatebleInstance)
{
	if(_updates[updatebleInstance->UpdID.Main]==updatebleInstance)
	{
		_updates[updatebleInstance->UpdID.Main] = NULL;
		--_NumberOfUpdateClients;
	}
}

void
UpdateManager::SignOutFromEarlyUpdate(IUpdateble* updatebleInstance)
{
	if(_earlyUpdates[updatebleInstance->UpdID.Early]==updatebleInstance)
	{
		_earlyUpdates[updatebleInstance->UpdID.Early] = NULL;
		--_NumberOfEarlyClients;
	}
}

void
UpdateManager::SignOutFromLateUpdate(IUpdateble* updatebleInstance)
{
	if(_lateUpdates[updatebleInstance->UpdID.Late] == updatebleInstance)
	{
		_lateUpdates[updatebleInstance->UpdID.Late] = NULL;
		--_NumberOfLateClients;
	}
}



void //Invoking all Update-Clients...
UpdateManager::DoTheUpdates(void)
{
	unsigned i,counter;
	
	i = counter = 0;
	while(counter<_NumberOfEarlyClients)
	{
		if(_earlyUpdates[i]!=NULL)
			{ _earlyUpdates[i]->DoEarly(); counter++; }

		i++;
	}

	INPUT->FireEvents();

	i = counter = 0;
	while(counter<_NumberOfUpdateClients)
	{
		if(_updates[i]!=NULL)
			{ _updates[i]->DoUpdate(); counter++; }

		i++;
	}

//#ifdef LATE_BEFOR_DRAW
//#undef LATE_AFTER_DRAW
//	i = counter = 0;
//	while(counter<_NumberOfLateClients)
//	{
//		if(_lateUpdates[i]!=NULL)
//			{ _lateUpdates[i]->DoLate(); counter++;	}
//
//		i++;
//	}
//#endif
}

//#ifdef LATE_AFTER_DRAW
void 
UpdateManager::DoTheLateUpdates(void)
{
	unsigned i,counter;
	i = counter = 0;
	while(counter<_NumberOfLateClients)
	{
		if(_lateUpdates[i]!=NULL)
			{ _lateUpdates[i]->DoLate(); counter++;	}

		i++;
	}
}
//#endif



  //////////////////////////
/*  IUpdateble-functions:  */
 //////////////////////////

IUpdateble::IUpdateble(void)
{
	UpdID.Early=UpdID.Late=UpdID.Main=NULL;
	this->InitiateUpdatable();
}

IUpdateble::~IUpdateble(void)
{
	if(UpdID.Early)
		UpdateManager::getInstance()->SignOutFromEarlyUpdate(this);
	if(UpdID.Main)
		UpdateManager::getInstance()->SignOutFromUpdate(this);
	if(UpdID.Late)
		UpdateManager::getInstance()->SignOutFromLateUpdate(this);
}

void
IUpdateble::InitiateUpdatable(void)
{
	UpdateManager::getInstance()->SignInForUpdate(this);
}
