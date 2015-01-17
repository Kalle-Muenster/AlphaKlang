#include "Transform.h"

const ITransform _ZeroTransformInstance = ITransform(); 

//Zero-Initialized-instance:

const ITransform*
	const ITransform::Zero = &_ZeroTransformInstance;

ITransform::ITransform(void)
{		

	dimension.position = dimension.rotation = physics.movement = Vector3(0,0,0);
	dimension.scale = Vector3(1,1,1);
	direction.forward = Vector3(0,0,-1);
	direction.right = Vector3(1,0,0);
	direction.up = Vector3(0,1,0); 
	physics.depth = 1;
	physics.thickness = 0.5;
	physics.speed = Controlled<float>();
	physics.speed.SetUp(Controlled<float>::Cycle,-1,1,0,0.005);
	physics.speed.Mode(Controlled<float>::Cycle);
	physics.mass = Controlled<float>();
	physics.mass.SetUserMode<Calculator<float,3>>(-1000,1000,0.5f,0);
	physics.mass.GetUserMode<Calculator<float,3>>()->pPIN[0] = &dimension.scale.x;
	physics.mass.GetUserMode<Calculator<float,3>>()->pPIN[1] = &dimension.scale.y;
	physics.mass.GetUserMode<Calculator<float,3>>()->pPIN[2] = &dimension.scale.z;
	physics.mass.GetUserMode<Calculator<float,3>>()->pTARGET=&physics.depth;
	dimension.DimensionDirty = direction.DirectionsDirty = physics.PhysicsDirty = false;
}

ITransform::~ITransform(void)
{

}

ControlledFloat::ControlledFloat(void)
{
	MOVE = 0;
	MIN = 0;
	MAX = 1;
	_Value = new float(0.5f);
	Mode(ControlledFloat::ControllMode::Clamp);
}
ControlledFloat::~ControlledFloat(void)
{
	delete _Value;
}

float
ControlledFloat::checkValue(unsigned short mode)
{
	switch (mode)
	{
	case 0:
		return *_Value;
	case ControlledFloat::ControllMode::Invert:
		 return -(*_Value);
	break;
	case ControlledFloat::ControllMode::Clamp:
		*_Value = *_Value<MIN?MIN:*_Value>MAX?MAX:*_Value; 
	break;

	case ControlledFloat::ControllMode::Cycle:
		*_Value = *_Value<MIN?MAX-(MIN- *_Value):*_Value>MAX?MIN+(*_Value - MAX): (*_Value+MOVE); 
	break;
	case ControlledFloat::ControllMode::PingPong:
		*_Value = *_Value<MIN?(MIN+(MIN- *_Value))+(MOVE *= -1):*_Value>MAX? (MAX-(*_Value-MAX))+(MOVE *= -1) :(*_Value+MOVE); 
	break;
	default:
		if(UserMode->ID==mode)
			return UserMode->checkVALUE(_Value);
	}

	return *_Value;
}

Controlled<float>*
ControlledFloat::SetUp(Controlled<float>::ControllMode mode,float initial)
{
	*_Value=initial;
	Mode(mode);
	return this;
}

Controlled<float>*
	ControlledFloat::SetUp(Controlled<float>::ControllMode mode,float min,float max,float move,float initial)
{
	MIN=min;
	MAX=max;
	*_Value = initial;
	Mode(ControlledFloat::ControllMode::Clamp);
	return this;
}

float
ControlledFloat::operator=(float setter)
{
	return SetValue(setter);
}