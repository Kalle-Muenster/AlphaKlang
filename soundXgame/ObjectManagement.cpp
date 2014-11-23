#include "ObjectManagement.h"
#include "projectClasses.h"

ObjectManagement*
ObjectManagement::This;

ObjectManagement::ObjectManagement(void)
{
}


ObjectManagement::~ObjectManagement(void)
{
}

ObjectManagement*
ObjectManagement::getInstance(void)
{
	if(This==NULL)
		This = new ObjectManagement();
	return This;
}