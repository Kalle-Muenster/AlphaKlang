#include "ControllElement.h"


ControllElement::ControllElement(void)
{

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
	left=Area.GetCenter().x-Area.GetHalbSize().x;
	right=Area.GetCenter().x+Area.GetHalbSize().x;
	top=Area.GetCenter().y-Area.GetHalbSize().y;
	bottom=Area.GetCenter().y+Area.GetHalbSize().y;
	return Area;
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