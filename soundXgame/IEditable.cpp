#include "IEditable.h"
#include <string.h>

void BackButton(IConnectable* sender)
{
	sender->Connection()->isVisible(false);
}

void _scaleFunc(IConnectable* sender)
{
	if(((ControllElement*)sender)->GetText()=="Scale x")
		printf("yeah");	
}

IEditable::IEditable(void)
{
	Bindings.push_back(BINDING());

	strcpy_s(&Bindings[0].labelName[0], sizeof(&Bindings[0].labelName[0]),"Back");
	
	Bindings[0].TypeInfo = &typeid(bool);
	Bindings[0].VariableData =&((IObjection<IConXtor>*)this)->IsVisible;
	Bindings[0].OnChange = BackButton;




}

IEditable::~IEditable(void)
{

}

void 
IEditable::BindMenuObject(GuiObject* menu)
{
	panel=menu;
}

short 
IEditable::BindValueToMenu(bool& VariableToBind,action Handler,string name)
{
	int index = Bindings.size();
	if(index<10)
	{
		Bindings.push_back(BINDING());
		strcpy_s(&Bindings[index].labelName[0], sizeof(&Bindings[index].labelName[0]),name);
		Bindings[index].TypeInfo = &typeid(bool);
		Bindings[index].VariableData = &VariableToBind;
		Bindings[index].OnChange = Handler;
		return index;
	}
	return -1;
}

short 
IEditable::BindValueToMenu(float& VariableToBind,action handler,string name)
{
	int index = Bindings.size();
	if(index<10)
	{
		Bindings.push_back(BINDING());
		strcpy_s(&Bindings[index].labelName[0], sizeof(&Bindings[index].labelName[0]),name);
		Bindings[index].TypeInfo = &typeid(float);
		Bindings[index].VariableData = &VariableToBind;
		Bindings[index].OnChange = handler;
		return index;
	}
	return -1;
}

bool 
IEditable::ShowDialog(void)
{
	if(panel)
	{
		short numberOfControlls = Bindings.size();
		panel->Area.SetSize(256,10+60*numberOfControlls);
		for(short index = 0; index<Bindings.size();index++)
		{
			if(panel->GetConnected<ControllElement>(index)!=NULL)
				panel->Remove<ControllElement>(index);

			if(Bindings[index].TypeInfo==&typeid(bool))
			{
				panel->AddConnectable<ButtonControl>()->ClickAction = Bindings[index].OnChange;
				panel->GetConnected<ButtonControl>(index)->SetSwitch((bool*)Bindings[index].VariableData);

			}
			else if(Bindings[index].TypeInfo==&typeid(float))
			{
				panel->AddConnectable<Knob>()->ClickAction = Bindings[index].OnChange;
				panel->GetConnected<Knob>(index)->Value.SetVariable((float*)Bindings[index].VariableData);
			}

			panel->GetConnected<ControllElement>(index)->SetText(&Bindings[index].labelName[0]);	
			panel->GetConnected<ControllElement>(index)->SizeScaledPanel = (VectorF(0.5,1.f/numberOfControlls) * 0.9);
			panel->GetConnected<ControllElement>(index)->PositionOnPanel = VectorF(10,5+(60*index));
		}
		return panel->isVisible(true);

	//	glutSetCursor(GLUT_CURSOR_INHERIT);
	}
}



void 
IEditable::HideDialog(void)
{
	if(panel)
	{
		if(panel->IsVisible)
		{
			for(short i=1;i<11;i++)
				panel->Remove<ControllElement>(i);
			panel->isVisible(false);
		}
	//   glutSetCursor(GLUT_CURSOR_NONE);
	}
}


std::vector<BINDING>*
IEditable::GetDialogBindings(void)
{
	return &Bindings;
}