#include "primitivSwitcher.h"
#include "PrimitivObject.h"
#include "Camera.h"
 

primitivSwitcher::primitivSwitcher(void)
{
	currentSellected = CUBE;
	switched = false;
}


primitivSwitcher::~primitivSwitcher(void)
{
	UPDATE->SignOutFromUpdate(this);
}


bool
primitivSwitcher::Initialize(void)
{
	return true;
}

void 
primitivSwitcher::DoUpdate(void)
{
	if(SCENE->camera->GetTarget())
	{
		if(SCENE->camera->GetTarget()->GetID()==this->Connection()->GetID())
		{
			 if(INPUT->Mouse.WheelV > 0)
			 {
				 if(++currentSellected > FLATQUAD)
					 currentSellected = CUBE;
				 switched=true;
			 }
			 else if(INPUT->Mouse.WheelV < 0)
			 {
				if(--currentSellected < CUBE)
					currentSellected = FLATQUAD;
				switched=true;
			 }

			 if(switched)
			 {

				 Switch((PRIMITIVES)currentSellected);
				 switched=false;
			 }

		}
	//	else
//			IsActive=false;
	}


}

void 
primitivSwitcher::Switch(PRIMITIVES prim)
{

		switch(currentSellected)
		{
		case CUBE:
			((IPrimitivObject*)Connection())->SetPrimitiv<ICubic>();
			break;
		case BALL:
			((IPrimitivObject*)Connection())->SetPrimitiv<IBall>();
			break;
		case SPHERE:
			((IPrimitivObject*)Connection())->SetPrimitiv<ISphere>();
			break;
		case ZYLINDER:
			((IPrimitivObject*)Connection())->SetPrimitiv<IZylinder>();
			break;
		case CONE:
			((IPrimitivObject*)Connection())->SetPrimitiv<ICone>();
			break;
		case CAPSULE:
			((IPrimitivObject*)Connection())->SetPrimitiv<ICapsule>();
			break;
		case FLATQUAD:
			((IPrimitivObject*)Connection())->SetPrimitiv<IFlatquad>();
			break;
		}

}