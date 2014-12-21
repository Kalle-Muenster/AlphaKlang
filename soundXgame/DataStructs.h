#ifndef __TRANSFORM__
#define __TRANSFORM__


#include <bass.h>
#include "projectGrafics.h"
#include "VoxelVector.h"
#include "List.h"

typedef char* string;



struct Vector3 
{
public:
	GLfloat x,y,z;
	Vector3(void);
	Vector3(float,float,float);
	Vector3(glm::vec3);
	Vector3(BASS_3DVECTOR);
	static const Vector3* const Zero;
	float dot(Vector3);
	Vector3 cros(Vector3);
	float distance(Vector3);
	float GetLength(void);
	Vector3 direction(Vector3 to);
	Vector3 normalized(void);
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
	GLfloat& operator[](int); 
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

//transform 
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



#endif