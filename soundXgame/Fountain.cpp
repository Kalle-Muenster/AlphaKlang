#include <stdio.h>
#include <time.h>

#include "Fountain.h"
#include "projectMacros.h"

Fountain::Fountain(void)
	: size(20), timer(0), delay(1.4f)
{
	this->spawnLeft = this->size;
	this->createRange();


	UpdateManager::getInstance()->SignInForUpdate(this);


}


Fountain::~Fountain(void)
{
	for (auto it = objects.begin(); it != objects.end(); ++it)
	{
		delete *it;
	}
}

/* Design Pattern -> Object Pool */
void
Fountain::setMaxSize(int i)
{
	size = i;
}

void
Fountain::createRange(void)
{
	for(int j = 0; j < this->size; j++)
	{
		FourtainObject* tmp = new FourtainObject();
		objects.push_back(tmp);
	}
}

FourtainObject*
Fountain::getObject(void)
{
	if(objects.size() >= 1)
	{
		FourtainObject* tmp = objects[0];
		objects.erase(objects.begin());
		return tmp;
	} else {
		return new FourtainObject();
	}
}

void
Fountain::release(FourtainObject* obj)
{
	obj->Clear(); // Clear method set values to 0 or null
	objects.push_back(obj);

}

void
Fountain::DoUpdate(void)
{
	timer += INPUT->FrameTime;

	if(spawnLeft != 0 && timer >= this->delay)
	{
		this->Spawn();
		timer = 0;
		spawnLeft--;
	}

	// TODO Finish Update
	if(spawnLeft == 0)
	{
		// SignOutForUpdate
		//UpdateManager::getInstance()->SignOutForUpdate(this);
	}

}

void
Fountain::Spawn(void)
{
	float randX = Utility::GetRandom() - 0.5f;
	float randZ = Utility::GetRandom() - 0.5f;

	FourtainObject* cube = Fountain::getObject();
	cube->move(1.0f, 2.0f, -3.0f);
	//cube->AddToScene();
	cube->IsGrounded = false;
	cube->scale(Vector3(0.3f,0.3f,0.3f));

	cube->power = 1.0f;
	cube->gravity = -0.015;
	cube->speed = 0.03f;
	cube->expanding = 5.0f; // ausweiten: 1=wenig,5=stark
	cube->direction = Vector3(randX,0,randZ);

}

void
Fountain::Draw(void)
{
}
