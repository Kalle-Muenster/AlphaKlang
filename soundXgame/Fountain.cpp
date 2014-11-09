#include "Fountain.h"


Fountain::Fountain(void) : size(10)
{

	this->createRange();

	cube = Fountain::getObject();
	//cube->scale(Vector3(2,2,2));


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

	FourtainObject* tmp = objects[0];
	objects.erase(objects.begin());
	return tmp;
}

void
Fountain::release(FourtainObject* obj)
{
	//obj->Clear(); // Clear method set values to 0 or null
	objects.push_back(obj);

}


/* Logic */
void
Fountain::Init(void)
{

}

void
Fountain::DoUpdate(void)
{
	cube->Update();
}

void
Fountain::Draw(void)
{
}
