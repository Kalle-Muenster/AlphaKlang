#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

//#include "Connectable.h"
#include "ControlledValues.h"



struct IDimension
{

	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	bool DimensionDirty;				// true if this Dimensions have Changed and the Directions need to be Updated..
			// Updates the Dimensions...
};

struct IDirection
{

	Vector3 forward;
	Vector3 right;
	Vector3 up;

	bool DirectionsDirty;				// true if the Directions have unUpdated changes...
	
};

struct IPhysic
{
	Vector3 movement;			// movement is a relative position and shows the difference to the last frame -> it's the Object's moving direction * speed
	Controlled<float> speed;		// moving-speed
	float depth;				// amount of matter per qm... 1 means 1Kg per 1m^3
	float thickness;			// the thicker the harder... 1 means indistructable, 0 means cookie

	Controlled<float> mass;			// the Weight of an object: depth*scale;

	bool PhysicsDirty;			// physicaly dirty... 

};

class ITransform 
{
public:
IDimension dimension;
IDirection direction;
IPhysic	   physics;
								ITransform(void);
virtual							~ITransform(void);
bool							IsDirty(void);
void							SetClean(void); 
bool							CleanDimensions(void);
bool							CleanDirections(void);		// Get's the dirt off...
bool							SetPhysicClean(void);	// cleanUp...
};

#endif