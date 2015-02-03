#ifndef _OBJECTMANAGEMENT_H_
#define _OBJECTMANAGEMENT_H_

//#ifndef _POJECT_MACROS__
//#define OBJECT ObjectManagement::getInstance()
//#endif

#include "Connectable.h"
#include "ControllElement.h"

class ObjectManagement
{
private:
	static bool NotIsInstanciated;
	ObjectManagement(void);
	std::vector<unsigned> usedIDs;
public:
	static const char* start(void);
	static ObjectManagement* getInstance(void);
	~ObjectManagement(void);
	unsigned GenerateID(void);
	void FreeID(unsigned);
	IObjection<IConXtor>* Find(unsigned ID);
	IObjection<IConXtor>* Find(char* name);
	ControllElement*	  FindGUIControll(char* PanelName,char* label);

};

#endif