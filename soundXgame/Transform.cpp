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
//	physics.speed.SetUp(-1,1,0,0.005,physics.speed.Cycle);
	physics.speed.Mode(physics.speed.Cycle);
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

