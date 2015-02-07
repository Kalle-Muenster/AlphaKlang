#ifndef _INTERFACE_IEditable_
#define _INTERFACE_IEditable_

#include "GuiControlls.h"

struct BINDING
{
	const type_info*	TypeInfo;
	void*				VariableData;
	char				labelName[32];
	action				OnChange;
};

class IEditable
{
protected:
	std::vector<BINDING>	Bindings;
	GuiObject*				panel;


public:
				IEditable(void);
	virtual	   ~IEditable(void);
	void		BindMenuObject(GuiObject*);
	short		BindValueToMenu(float& VariableToBind,action,string);
	short		BindValueToMenu(bool& VariableToBind,action,string);
	void		ShowDialog(void);
	void		HideDialog(void);
	std::vector<BINDING>*  GetDialogBindings(void);
};

#endif
