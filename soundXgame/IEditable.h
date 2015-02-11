#ifndef _INTERFACE_IEditable_
#define _INTERFACE_IEditable_

#include "GuiControlls.h"

class ObjectMenu;

struct ControlElementBinding
{
	enum ELEMENT_TYPE : char
	{BUTTON=0,SLIDER=1,KNOB=2};

	ELEMENT_TYPE		ControlType;
	void*				VariablePointer;
	char				Label[32];
	action				OnChangeAction;
};

class IEditable
{
protected:
	std::vector<ControlElementBinding>	Bindings;
	ObjectMenu*							GUIPanel;

public:
					IEditable(void);
	virtual		   ~IEditable(void);
	virtual bool	ShowDialog(void);//IObjection<IConnectable>* thisObject);
	void			HideDialog(void);
	unsigned		AddControlElement(ControlElementBinding::ELEMENT_TYPE controllType,void* targetVariable,const char* lable,action handler);
	void			RemoveControllElement(unsigned controlID);
	bool			isAttachedTo(ObjectMenu* menu);
};

#endif
