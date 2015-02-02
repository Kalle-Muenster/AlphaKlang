#ifndef __TRANSFORM__
#define __TRANSFORM__


#include <bass.h>
#include "projectGrafics.h"
#include "VoxelVector.h"
#include "List.h"

typedef char*          string;
typedef unsigned int   GobID;
typedef unsigned int ConID;
//typedef short          BOOL;
//typedef void           (*action)(IConnectable*);

struct Texture 
{
	enum Format
	{DUAL=1,GRAY=16,RGB=24,RGBA=GL_RGBA,BGRA=GL_BGRA_EXT,ABGR=GL_ABGR_EXT};
	GLuint ID;
	short w,h;
	GLint format;
	void* pData;

	void Loade(string filename);
	void Loade(string fileName,short Width,short Height,Format = RGBA);
	Texture operator =(Texture setter)
	{
		ID = setter.ID;
		h=setter.h;
		w=setter.w;
		format = setter.format;
		pData = setter.pData;
		return *this;
	}
};

template<const int NUMBER_OF_FRAMES>  
class MultilayerTexture
	: public Texture
{
private:
	GLuint frameIDs[NUMBER_OF_FRAMES];
	unsigned short frameDurations[NUMBER_OF_FRAMES];
	double timer;
	int current;

public:
	MultilayerTexture(int fps = -1)
	{
		current = 0;
		timer = 1000/fps;
		 if(fps>0)
		 {
			 for(int i=0;i<NUMBER_OF_FRAMES;i++)
				frameDurations[i]=timer;
		 }
	}
	virtual ~MultilayerTexture(void){}
	void LoadeFrames(string filenames[])
	{
		for(int i =0; i<NUMBER_OF_FRAMES ; i++)
		{
			frameIDs[i] = Utility::loadTexture(filenames[i]);
		}
	}
	void SetFrame(int frameNumber)
	{
		 current=frameNumber;
		 ID = frameIDs[current];
	}
	void Animate(void)
	{
		if((timer+=INPUT->FrameTime) > frameDurations[current])
		{
			timer=0;
			if(++current==NUMBER_OF_FRAMES)
				current=0;

			ID=frameIDs[current];
		}
	}
};

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
	float GetPowerOfLength(void);
	Vector3 direction(Vector3 to);
	Vector3 normalized(void);
	operator BASS_3DVECTOR();
	operator glm::vec3();
	bool operator==(Vector3);
	bool operator==(float);
	bool operator!=(Vector3);
	bool operator!=(float);
	bool operator<(Vector3);
	bool operator>(Vector3);
	Vector3 operator+(Vector3);
	Vector3 operator-(Vector3);
	Vector3 operator-(void);
	Vector3 operator*(float);
	Vector3 operator/(float);
	void operator +=(Vector3);
	void operator -=(Vector3);
	void operator *=(float);
	void operator /=(float);
	GLfloat* operator[](int); 
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
	data32 data;
	virtual void BaseInitor(void);
public:
	Data32(void);
	Union32 GetUnion(void);
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