#include "FourtainObject.h"
#include "projectMacros.h"

FourtainObject::FourtainObject(void) : power(1)
{


	InitializeObject("cube-quads.obi",false);
	this->LoadTexture("X-512.jpg");

	
	this->IsVisible=true;

	this->move(1.0f, 2.0f, -3.0f);
	this->scale(Vector3(0.3f,0.3f,0.3f));
}


FourtainObject::~FourtainObject(void)
{
}

void
FourtainObject::Create(void)
{
	SCENE->Add(this);
}

void
FourtainObject::Clear(void)
{
	//SCENE->remove(this);
}

void
FourtainObject::Update(void)
{
//	float deltaTime = 0.005;

//	timeSinceStart += deltaTime;
//	power -= deltaTime;

	timeSinceStart += INPUT->FrameTime; //INPUT->FrameTime hold's actual deltaTime in seconds
	power -= INPUT->FrameTime;

	Transform transform = this->transform;
	Vector3 pos = transform.position;
	pos.y += 0.01f;
	this->move(pos);

}