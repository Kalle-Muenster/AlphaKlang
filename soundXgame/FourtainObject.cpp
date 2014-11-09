#include "FourtainObject.h"


FourtainObject::FourtainObject(void) : power(1)
{


	InitializeObject("cube-quads.obi",true);
	this->LoadTexture("X-512.jpg");

	
	this->IsVisible=true;

	this->move(1.0f, 2.0f, -3.0f);
}


FourtainObject::~FourtainObject(void)
{
}

void
FourtainObject::Update(void)
{
	float deltaTime = 0.005;

	timeSinceStart += deltaTime;

	power -= deltaTime;

	Transform transform = this->transform;
	Vector3 pos = transform.position;
	pos.y += 0.01f;
	this->move(pos);

}