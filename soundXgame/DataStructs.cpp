

#include "DataStructs.h"


Vector3::Vector3(void)
{
	x=y=z=0.0f;
}

Vector3::Vector3(float X,float Y,float Z)
{
	bassVector.x = x = X;
	bassVector.y = y = Y;
	bassVector.z = z = Z;
}
Vector3::Vector3(glm::vec3 glVector)
{
	bassVector.x = x = glVector.x;
	bassVector.y = y = glVector.y;
	bassVector.z = z = glVector.z;
}
Vector3::operator BASS_3DVECTOR()
{
	return BASS_3DVECTOR(this->x,this->y,this->z);
}

Vector3
Vector3::operator+(Vector3 adder)
{
	return Vector3(this->x + adder.x,this->y + adder.y,this->z + adder.z);
}

Vector3
Vector3::operator-(Vector3 subtractor)
{
	return Vector3(this->x - subtractor.x,this->y - subtractor.y,this->z - subtractor.z);
}

BASS_3DVECTOR*
Vector3::asBassVector(void)
{
	bassVector.x = this->x;
	bassVector.y = this->y;
	bassVector.z = this->z;
	return &bassVector;
}
