#ifndef __TRANSFORM__
#define __TRANSFORM__

#include <GL\freeglut.h>
#include <glm.hpp>
#include <bass.h>

typedef char* string;

struct Vector3 
{
public:
	GLfloat x,y,z;
	Vector3(void);
	Vector3(float,float,float);
	Vector3(glm::vec3);
	Vector3(BASS_3DVECTOR);
	float distance(Vector3);
	float GetLength(void);
	Vector3 direction(Vector3 to);
	BASS_3DVECTOR asBassVector(void);
	operator BASS_3DVECTOR();
	operator glm::vec3();
	bool operator==(Vector3);
	bool operator!=(Vector3);
	Vector3 operator+(Vector3);
	Vector3 operator-(Vector3);
	Vector3 operator*(float);
	Vector3 operator/(float);
	void operator +=(Vector3);
	void operator -=(Vector3);
};

struct Vector4 : public Vector3
{
public:
	GLfloat w;
};

union data32
{
	unsigned u32;
	float f32;
	short s16[2];
	unsigned char byte[4];
};

typedef data32* Union32;

struct Data32
{
protected:
	
	virtual void BaseInitor(void);
public:
	data32 data;
	Data32(void);
	data32 GetUnion(void);
	virtual ~Data32(void);
};



typedef short* pcm;

class Sample : Data32
{
protected:
	virtual void initor(void);
public:
	Sample(void);
	Sample(short Left,short Right);
	Sample(float mono);
	pcm L,R;
	float* Float32;
	short GetMono(void);
	pcm operator[] (int index);
	~Sample(void);
};

typedef unsigned char* chan;

class Color : Data32
{
protected:
	virtual void initor(void);
public:
	Color(void);
	Color(unsigned);
	Color(int r,int g,int b,int a);
	chan R,G,B,A;
	unsigned* uInt;
	chan operator[] (int index);
	~Color(void);
};


union Data64
{ 
	unsigned long u64;
	int s32[2];
	unsigned char byte[8];
	short s16[4];
	data32 unionData32[2];
	Data64(void);
};

struct Transform
{
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
	Vector3 movement;
	float speed;

	Vector3 forward;
	Vector3 right;
	Vector3 up;

};

struct IDimension
{
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;

	bool DimensionDirty;				// true if this Dimensions have Changed and the Directions need to be Updated..
	bool CleanDimensions(void);		// Updates the Dimensions...
};

struct IDirection
{
	Vector3 forward;
	Vector3 right;
	Vector3 up;

	bool DirectionsDirty;				// true if the Directions have unUpdated changes...
	bool CleanDirections(void);		// Get's the dirt off...
};

struct IPhysic
{
	Vector3 movement;			// movement is a relative position and shows the difference to the last frame -> it's the Object's moving direction * speed
	float speed;				// moving-speed
	float depth;				// amount of matter per qm... 1 means 1Kg per 1m^3
	float thickness;			// the thicker the harder... 1 means indistructable, 0 means cookie

	float mass(void);			// the Weight of an object: depth*scale;

	bool PhysicsDirty;			// physicaly dirty... 
	bool SetPhysicClean(void);	// cleanUp...
};

class FullTransform :
	IDimension,
	IDirection,
	IPhysic
{
public:
	bool IsDirty(void);
	void SetClean(void);

};






#endif