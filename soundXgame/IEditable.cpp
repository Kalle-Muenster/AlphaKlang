#include "IEditable.h"
#include "Cam.h"
#include "PointAndClick.h"

void _OnBackButtonClick(IConnectable* sender)
{
	sender->Connection()->isVisible(false);
}


IEditable::IEditable(void)
{
	GUIPanel = (ObjectMenu*)GuiManager::getInstance()->Panel("Editor-Panel");
}

IEditable::~IEditable(void)
{

}

bool
IEditable::isAttachedTo(ObjectMenu* menu)
{
	return (menu->isAttachedTo(this));
}

bool 
IEditable::ShowDialog(void)//IObjection<IConnectable>* thisObject)
{
	 if(!GUIPanel->IsVisible)// && GUIPanel->isAttachedTo(this))
	 {
//		 if(Utility::StringCompareRecursive(GUIPanel->GetText(),((IObjection<IConXtor>*)this)->GetName())>=0)
//		 {
			 char i = -1;
			 GUIPanel->ResetHard(false);
			 GUIPanel->SetText(((IObjection<IConXtor>*)this)->GetName());
			 GUIPanel->scale(Vector3(200,80*Bindings.size(),1));
			 for(auto it=Bindings.begin();it!=Bindings.end();it++)
			 {
				 i++;
				 switch(it->ControlType)
				 {
				 case ControlElementBinding::ELEMENT_TYPE::BUTTON:
					 GUIPanel->AddConnectable<ButtonControl>()->SetSwitch((bool*)it->VariablePointer);
					 break;
				 case ControlElementBinding::ELEMENT_TYPE::SLIDER:
					 GUIPanel->AddConnectable<SliderX>()->ValueX.SetVariable((float*)it->VariablePointer);
					 break;
				 case ControlElementBinding::ELEMENT_TYPE::KNOB:
					 GUIPanel->AddConnectable<Knob>()->Value.SetVariable((float*)it->VariablePointer);
					 break;
				 }

				 GUIPanel->GetConnected<ControllElement>(i)->SetText(&it->Label[0]);
				 GUIPanel->GetConnected<ControllElement>(i)->PositionOnPanel = VectorF(10,10+i*80);
				 GUIPanel->GetConnected<ControllElement>(i)->SizeScaledPanel = VectorF(0.9,1.0f/i);
//			 }
		 }
		 SceneGraph::getInstance()->camera->Mode(POINTANDCLICK);
		 GUIPanel->isVisible(true);
		 return true;
	 }
	 return false;
}

void 
IEditable::HideDialog(void)
{
	GUIPanel->isVisible(false);
	SceneGraph::getInstance()->camera->Mode(POINTANDCLICK);
}

unsigned 
IEditable::AddControlElement(ControlElementBinding::ELEMENT_TYPE controllType,void* targetVariable,const char* lable,action handler)
{
	char c=0;
	ControlElementBinding element = ControlElementBinding();
	element.ControlType = controllType;
	element.VariablePointer = targetVariable;
	while((element.Label[c]=lable[c])!='\0' && c<32)c++;
	element.OnChangeAction = handler;

	Bindings.push_back(element);
	return Bindings.size()-1;
}

void
IEditable::RemoveControllElement(unsigned controlID)
{
	Bindings.emplace_back(Bindings.at(controlID));
	Bindings.pop_back();
}