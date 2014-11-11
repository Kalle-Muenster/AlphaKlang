#include "Transform.h"

ITransform::ITransform(void)
{
	position = rotation = movement = Vector3(0,0,0);
	scale = Vector3(1,1,1);
	forward = Vector3(0,0,-1);
	right = Vector3(1,0,0);
	up = Vector3(0,1,0); 
	depth = 1;
	thickness = 0.5;
	speed = *ControlledFloat().SetUp(-10,100,0);
	DimensionDirty = DirectionsDirty = PhysicsDirty = false;
}

ITransform::~ITransform(void)
{

}

ControlledFloat::ControlledFloat(void)
{
	MIN = 0;
	MAX = 1;
	_Value = new float(0.5f);
	Mode(ControlledFloat::ControllMode::Clamp);
}

float
ControlledFloat::checkValue(ControlledFloat::ControllMode mode)
{
	switch (mode)
	{
	case ControlledFloat::ControllMode::Clamp:
		 *_Value = *_Value<MIN?MIN:*_Value>MAX?MAX:*_Value; 
	break;
	}

	return *_Value;
}

ControlledFloat*
ControlledFloat::SetUp(ControlledFloat::ControllMode mode,float initial)
{
	*_Value=initial;
	Mode(mode);
	return this;
}

ControlledFloat*
ControlledFloat::SetUp(float min,float max,float initial)
{
	MIN=min;
	MAX=max;
	*_Value = initial;
	Mode(ControlledFloat::ControllMode::Clamp);
	return this;
}