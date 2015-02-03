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
		GUI->Remove(GUI->Element(id));
	}
	else
	{
		SCENE->Remove(SCENE->Object(id));
	}
}

IObjection<IConXtor>* 
ObjectManagement::Find(unsigned ID)
{
	IObjection<IConXtor>* obj;
	if(ID<MAX_MUM_SCENE_OBJECTS)
		obj = (IObjection<IConXtor>*)SCENE->Object(ID);
	else if	(ID<_managedIDs)
		obj = (IObjection<IConXtor>*)GUI->Element(ID);
	else
		{
			//todo
		}

	return obj;
}

IObjection<IConXtor>* 
ObjectManagement::Find(char* name)
{
	 IObjection<IConnectable>* obj = SCENE->Object(name);
	 if(obj==NULL)
		 obj = GUI->Element(name);

	 return (IObjection<IConXtor>*)obj;
}

ControllElement*
ObjectManagement::FindGUIControll(char* PanelName,char* label)
{
	GuiObject* panel = (GuiObject*)GUI->Element(PanelName);
	for(unsigned c = 1;c<panel->conXtor->GetNumberOfConnected();c++)
		{
			 ControllElement* cnt = panel->GetConnected<ControllElement>(c);
			 if(cnt->GetText()==label)
				 return cnt;
		}
	return NULL;
}

