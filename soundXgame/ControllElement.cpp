#include "ControllElement.h"


ControllElement::ControllElement(void)
{
	  Label[0]='\0';
	  SetColor(255,255,0,255);
}

bool
ControllElement::isVisible(BOOL setter)
{
	if(setter==3)
		IsUpdatingActive = IsVisible = this->Connection()->isVisible(setter);
	else
		IsVisible = setter;

	return IsVisible; 
}

ControllElement::~ControllElement(void)
{

}

//bool
//ControllElement::Initialize(void)
//{
//	if(connection)
//		Panel = ProjectMappe::Rectangle(&this->Connection()->getTransform()->position.x,
//										&this->Connection()->getTransform()->position.y,
//										&this->Connection()->getTransform()->scale.x,
//										&this->Connection()->getTransform()->scale.y );
//	return (bool)connection;
//}

ProjectMappe::Rectangle
ControllElement::GetArea(void)
{
	VectorF	vec = Panel.GetSize();
	vec.x *= SizeScaledPanel.x;
	vec.y = vec.x/4;
	Area.SetSize(vec);
	vec = (Panel.GetPosition() + PositionOnPanel );
	Area.SetPosition(vec);
	left=vec.x;
	right=Area.GetCenter().x+Area.GetHalbSize().x;
	top=vec.y;
	bottom=Area.GetCenter().y+Area.GetHalbSize().y;
	return Area;
}



char* 
ControllElement::GetText(void)
{
	return &Label[0];
}

void
ControllElement::SetText(char* label)
{
	Label[63]='\0';
	short i = -1;
	while((Label[++i]=label[i])!='\0' && (i<64));
	
}

unsigned 
ControllElement::GetElementID(void)
{
   return Connection()->GetID()+this->ConnectionID;
}