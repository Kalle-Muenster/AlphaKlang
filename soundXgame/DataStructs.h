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
public:
	Vector3 position;
	Vector3 scale;
	BASS_3DVECTOR movement; // movement is a relative position and shows the difference to the last frame
	Vector3 rotation;

	Vector3 forward;
	Vector3 right;
	Vector3 up;
};






#endif