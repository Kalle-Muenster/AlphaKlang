#include <stdio.h>
#include <time.h>

#include "Fountain.h"
#include "projectMacros.h"

Fountain::Fountain(void)
	: size(10), timer(0)
{
	this->spawnLeft = this->size;

	this->createRange();

	

	//tmp->x = 10.123;
	//Fountain::release(tmp);
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
	float delay = 0.4f;
	timer += INPUT->FrameTime;

	if(spawnLeft != 0 && timer >= delay)
	{
		this->Spawn();
		timer -= delay;
		spawnLeft--;
	}

	// TODO Finish Update
	if(spawnLeft == 0)
	{
		// SignOutForUpdate
	}

}

void
Fountain::Spawn(void)
{
	float randX = Utility::GetRandom();;
	float randZ = Utility::GetRandom();;
	//float randSpeed = Utility::GetRandom();;

	

	FourtainObject* cube = Fountain::getObject();
	cube->move(1.0f, 2.0f, -3.0f);
	cube->AddToScene();

	cube->power = 1.0f;
	cube->gravity = -0.001;
	cube->speed = 0.01f;
	cube->expanding = 100; // ausweiten
	cube->direction = Vector3(randX,0,randZ);

}

void
Fountain::Draw(void)
{
}
