#include <stdio.h>
#include <time.h>

#include "Fountain.h"
#include "projectMacros.h"

Fountain::Fountain(void) :
	timer(0),
	timer2(0),
	spawnDelay(0.05f),
	deleteDelay(1.5f),
	size(10)
{
	this->transform.position = Vector3(0, 0, 0);
	UpdateManager::getInstance()->SignInForUpdate(this);
}

Fountain::~Fountain(void)
{
	for (auto it = objects.begin(); it != objects.end(); ++it)
	{
		delete *it;
	}
	for (auto it = spawnedObjects.begin(); it != spawnedObjects.end(); ++it)
	{
		delete *it;
	}
}

void
Fountain::SetPosition(Vector3 pos)
{
	this->transform.position = pos;
}

void
Fountain::createRange(void)
{
	for(int j = 0; j < size; j++)
	{
		FountainObject* tmp = new FountainObject();
		objects.push_back(tmp);
	}
}

FountainObject*
Fountain::getObject(void)
{
	FountainObject* tmp;
	if(objects.size() >= 1)
	{
		tmp = objects[0];
		objects.erase(objects.begin());
		//std::cout<<"Fountain... old!\n";
	} else {
		tmp = new FountainObject();
		//std::cout<<"Fountain spawned... CREATE!\n";
	}
	spawnedObjects.push_back(tmp);
	return tmp;
}

void
Fountain::release(FountainObject* obj)
{
	// Clear method set values to 0 or null
	obj->Clear();

	// set obj back into pool
	objects.push_back(obj);
}

void
Fountain::DoUpdate(void)
{
	timer += INPUT->FrameTime;

	//std::cout << motivator[0] << std::endl;

	if(motivator[0] > 0.75f)
	{
		if(timer >= this->spawnDelay)
		{
			this->Spawn();
			timer = 0;
		}
	}

	// Check removing old objects
	timer2 += INPUT->FrameTime;
	if(timer2 >= this->deleteDelay)
	{
		timer2 = 0;

		if(spawnedObjects.size() >= 1)
		{
			for (int i = (int)spawnedObjects.size() -1; i >= 0; --i)
			{
				FountainObject* it = spawnedObjects[i];
				float y = it->getTransform()->position.y;
				if(y < -2.0f)
				{
				
					this->release(spawnedObjects[i]);
				
					spawnedObjects.erase(spawnedObjects.begin() + i);

				}
			}
		}

	}


}

void
Fountain::Spawn(void)
{
	FountainObject* obj = Fountain::getObject();
	Vector3 pos = this->transform.position;
	pos.y += Ground::getInstance()->GetGroundY(pos.x, pos.z);
	obj->move(pos);
	obj->Initialize();
}

void
Fountain::Draw(void)
{
}
