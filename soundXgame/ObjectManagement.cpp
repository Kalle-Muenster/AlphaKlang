#include "projectMacros.h"
#include "ObjectManagement.h"
#include "projectClasses.h"

unsigned const		_managedIDs = MAX_MUM_SCENE_OBJECTS + MAX_NUM_GUI_OBJECTS -1;

unsigned			_counter = _managedIDs;

bool				
ObjectManagement::NotIsInstanciated = true;

ObjectManagement::ObjectManagement()
{
	NotIsInstanciated = false;
	usedIDs = std::vector<unsigned>();
}


ObjectManagement::~ObjectManagement(void)
{
}




unsigned
ObjectManagement::GenerateID(void)
{
	usedIDs.push_back(++_counter);
	return _counter;
}

void
ObjectManagement::FreeID(unsigned id)
{
	if(id > _managedIDs)
	{
		for(auto it=usedIDs.begin();it!=usedIDs.end();it++)
		{
			if((*it)==id)
			{
				usedIDs.emplace(it,usedIDs.back());
				usedIDs.pop_back();
				return;
			}
		}
	}
	else if(id >= MAX_MUM_SCENE_OBJECTS)
	{
		GUI2D->Remove(GUI2D->Element(id));
	}
	else
	{
		SCENE->Remove(SCENE->Object(id));
	}
}

