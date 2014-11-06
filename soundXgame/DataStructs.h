#ifndef __TRANSFORM__
#define __TRANSFORM__

#include <glm.hpp>
#include <bass.h>

typedef char* string;

struct Vector3 
	: public glm::vec3
{
private:
	BASS_3DVECTOR bassVector;
public:
	Vector3(void);
	Vector3(float,float,float);
	Vector3(glm::vec3);
	operator BASS_3DVECTOR();
	Vector3 operator+(Vector3);
	Vector3 operator-(Vector3);
	BASS_3DVECTOR* asBassVector(void);
};

/*struct TransformGL 
{
public:
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;

	glm::vec3 forward,right,up;
};*/

struct Transform
{
public:
	Vector3 position;
	Vector3 scale;
	BASS_3DVECTOR movement;
	Vector3 rotation;

	Vector3 forward;
	Vector3 right;
	Vector3 up;
};






#endif