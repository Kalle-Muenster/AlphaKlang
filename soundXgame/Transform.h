#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

//#include "Connectable.h"
#include "DataStructs.h"





template<typename ctrlType> class ControlledValue
{
protected:
	ctrlType* _Value;
	bool ControllerActive;
	enum struct ControllMode{NONE=0,Invert=1,Clamp=2,Cycle=3,PingPong=4}_mode;
	virtual ctrlType checkValue(ControllMode mode)
	{
		return *_Value;
	}
public:

	virtual ctrlType Value(ctrlType setter=_Nan._Float)
	{
		if(setter!=_Nan._Float)
		{
			*_Value = setter;
		}
		return checkValue(Mode());
	}

	ControlledValue(void)
	{
		this->_Value = new ctrlType();
	}

	virtual ~ControlledValue(void)
	{
		delete _Value;
	}

	virtual operator ctrlType(void)
	{
		return *this->_Value;
	}

	virtual bool IsControlled(BOOL active=3)
	{
		if(active<2)
			ControllerActive=active;
		
		return ControllerActive;
	}

	virtual ControllMode Mode(ControllMode mode=ControllMode::NONE)
	{
		if(_mode!=ControllMode::NONE)
			_mode=mode;
		return _mode;
	}

	virtual ControlledValue* SetUp(ControllMode mode,ctrlType initial)
	{
		*_Value = initial;
		Mode(mode);
		return this;
	}

	virtual void SetVariable(ctrlType & var)
	{
		*this->_Value = var;
	}
};

class ControlledFloat : public ControlledValue<float>
{
protected:
	virtual float checkValue(ControlledValue::ControllMode mode);

	float MIN,MAX;
public:
	ControlledFloat(void);
	virtual ControlledFloat* SetUp(ControlledFloat::ControllMode mode,float initial);
	virtual ControlledFloat* SetUp(float MIN,float MAX,float initial);
//	virtual float Value(float=_Nan._Float);
};

struct IDimension
{
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	bool DimensionDirty;				// true if this Dimensions have Changed and the Directions need to be Updated..
	bool CleanDimensions(void);		// Updates the Dimensions...
};

struct IDirection
{
	Vector3 forward;
	Vector3 right;
	Vector3 up;

	bool DirectionsDirty;				// true if the Directions have unUpdated changes...
	bool CleanDirections(void);		// Get's the dirt off...
};

struct IPhysic
{
protected:
	float MAX_SPEED,MIN_SPEED;
public:
	Vector3 movement;			// movement is a relative position and shows the difference to the last frame -> it's the Object's moving direction * speed
	ControlledFloat speed;		// moving-speed
	float depth;				// amount of matter per qm... 1 means 1Kg per 1m^3
	float thickness;			// the thicker the harder... 1 means indistructable, 0 means cookie

	float mass(void);			// the Weight of an object: depth*scale;

	bool PhysicsDirty;			// physicaly dirty... 
	bool SetPhysicClean(void);	// cleanUp...
};

class ITransform :
	IDimension,
	IDirection,
	IPhysic
{
public:
	ITransform(void);
	virtual ~ITransform(void);
	bool IsDirty(void);
	void SetClean(void);
const static ITransform* const Zero; 
};

#endif