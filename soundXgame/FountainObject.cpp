#include "FountainObject.h"
#include "projectMacros.h"

FountainObject::FountainObject(void)
{
	InitializeObject("cube_quads.obi",true);
	this->LoadTexture("X-7.png");

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
	this->power = 2.5f;
	this->gravity = -0.4f;
	this->speed = 0.5f;
	this->expanding = 40.0f;
	this->reduction = 1.0f;
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
		power -= (float)INPUT->FrameTime * this->reduction;
		if(power < 0)
			power = 0;
	}

	Transform transform = this->transform;
	Vector3 pos = transform.position;
	pos.y += speed * power + gravity;
	pos += direction / 200 * expanding * power;
	this->move(pos);

}