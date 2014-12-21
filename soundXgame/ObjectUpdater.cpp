#include "ObjectUptater.h"
#include "projectMacros.h"

bool
ObjectUpdater::Initialize(void)
{
	callbacks[0]=NULL;
	callbacks[1]=NULL;
	callbacks[2]=NULL;
	return true;
}

ObjectUpdater::~ObjectUpdater(void)
{
	
}

void 
ObjectUpdater::DoEarly(void)
{
	if(callbacks[0])
		callbacks[0]();
}

void 
ObjectUpdater::DoUpdate(void)
{
	if(callbacks[1])
		callbacks[1]();
}

void
ObjectUpdater::DoLate(void)
{
	if(callbacks[2])
		callbacks[2]();
}

void
ObjectUpdater::SetUpdateCallback(UPdTIME update,UpdateCallback callback)
{
	if(update==EARLY)
		UPDATE->SignInForEarlyUpdate(this);
	else if(update==MAIN)
		UPDATE->SignInForUpdate(this);
	else if(update==LATE)
		UPDATE->SignInForLateUpdate(this);

	this->callbacks[(int)update] = callback;
}