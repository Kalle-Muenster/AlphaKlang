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

bool
ControllElement::Initialize(void)
{
	Area = *ProjectMappe::Rectangle::Zero;
	//UPDATE->SignOutFromUpdate(this);
	//UPDATE->SignInForEarlyUpdate(this);

	Panel = ProjectMappe::Rectangle(&this->Connection()->getTransform()->position.x,
									&this->Connection()->getTransform()->position.y,
									&this->Connection()->getTransform()->scale.x,
									&this->Connection()->getTransform()->scale.y );

	PositionOnPanel = Panel.GetHalbSize();
	SizeScaledPanel.x = 0.1;
	SizeScaledPanel.y = SizeScaledPanel.x/4;
	this->angle = 0;
	GetArea();
	return true;
}

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
	short i = 0;
	while((Label[i]=label[i])!='\0' && (i<64))
		{i++;}
	
}

unsigned 
ControllElement::GetElementID(void)
{
   return Connection()->GetID()+this->ConnectionID;
}