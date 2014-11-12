#include "FourtainObject.h"
#include "projectMacros.h"

FourtainObject::FourtainObject(void)
{
	InitializeObject("cube-quads.obi",false);
	this->LoadTexture("X-512.jpg");
	
	// verschiedene start vec3
	// different scale (0.1 - 0.3)
	// different power
	// different timerHeigher

	this->scale(Vector3(0.3f,0.3f,0.3f));
	this->IsGrounded = false;

	LockID();

	
	UpdateManager::getInstance()->SignInForUpdate(this);

}


FourtainObject::~FourtainObject(void)
{
}

void
FourtainObject::AddToScene(void)
{
	SCENE->Add(this);
	this->IsVisible=true;
}

void
FourtainObject::Clear(void)
{
	//SCENE->remove(this);
	// TODO reset Values
}

void
FourtainObject::DoUpdate(void)
{
	//int id = GetID();


//	float deltaTime = 0.005;

//	timeSinceStart += deltaTime;
//	power -= deltaTime;

	//timeSinceStart += INPUT->FrameTime; //INPUT->FrameTime hold's actual deltaTime in seconds
	if(power > 0)
	{
		power -= INPUT->FrameTime / 5;
		if(power < 0)
			power = 0;
	}

	Transform transform = this->transform;
	Vector3 pos = transform.position;
	pos.y += speed * power + gravity;
	pos += direction / expanding * power;
	this->move(pos);

}