#include "FountainObject.h"
#include "projectMacros.h"

FountainObject::FountainObject(void)
{
	InitializeObject("cube_quads.obi",true);
	this->LoadTexture("X-512.jpg");

	//this->IsGrounded(false);
	this->scale(Vector3(0.3f,0.3f,0.3f));

	this->AddConnectable<Randomover>();
	this->GetConnected<Randomover>()->SetMoving(false);
	this->GetConnected<Randomover>()->SetSpeed(10.0f);

}

FountainObject::~FountainObject(void)
{
}

void
FountainObject::Initialize(void)
{
	float randX = Utility::GetRandom() - 0.5f;
	float randZ = Utility::GetRandom() - 0.5f;

	//this->power = 3.0f;
	//this->gravity = -0.035f;
	//this->speed = 0.04f;
	this->power = 4.0f;
	this->gravity = -0.1f;
	this->speed = 0.1f;
	this->expanding = 6.0f; // expanding: 1=low,5=high
	this->direction = Vector3(randX,0,randZ);

	SetID(SCENE->Add(this));
	LockID();

	UPDATE->SignInForUpdate(this);

}

// reset Values for set back to object pool
void
FountainObject::Clear(void)
{
	// Scene disable renderer
	SCENE->Remove(this);
	
	//RESET transform
	this->transform.position = Vector3();

	// SignOutForUpdate
	UPDATE->SignOutFromUpdate(this);

}

void
FountainObject::DoUpdate(void)
{
	if(power > 0)
	{
		power -= (float)INPUT->FrameTime / 2.5f;
		if(power < 0)
			power = 0;
	}

	Transform transform = this->transform;
	Vector3 pos = transform.position;
	pos.y += speed * power + gravity;
	pos += direction / 200 * expanding * power;
	this->move(pos);

}