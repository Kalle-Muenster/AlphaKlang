#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <vector>
#include <iostream>

#include "projectGrafics.h"
#include "Voxel.h"









enum POWERS_OF_TWO : unsigned short
{
	_32=32,_64=64,_128=128,_256=256,_512=512,_1024=1024,_2048=2048,_4096=4096,_=NULL
};

union u32_2s16_4b8
{
	unsigned u32;
	signed   s32;
	signed short s16[2];
	unsigned char byte[4];
	u32_2s16_4b8(void);
	u32_2s16_4b8(int);
};




typedef u32_2s16_4b8* LoaderColor;

class Loader
{
private:
	unsigned _w,_h,_length;
	std::vector<u32_2s16_4b8> _Data;
public:
	LoaderColor data;
	Loader(void);
	Loader(const char* filename);
	~Loader(void);
	int width(int=NULL);
	int height(int=NULL);
	int count(void);
	LoaderColor Pixel(int);
	LoaderColor Pixel(int,int);
	void* LoadeFile(const char* filename);
};

class Utility 
{
private:
	Utility();
	virtual ~Utility();
public:
/* loadObj-function: 
 * loades vertices,uv's and normals from OBJ-file to given vectors and the Shape within it's faces are made up...
 * returns a string containing the Object's Name or Type if's specified in file.. */
	static char* loadObj(const char*,  std::vector<glm::vec3> &, std::vector<glm::vec2> &, std::vector<glm::vec3> &,GLuint &);
	static GLuint loadTexture(const char *);
	static void Rotate90(float partOf90,float & A,float & B);
//	static Vector3 LoadePPMBitmap(const char* filename,Voxel* buffer);

	static float GetRandom(void); // Returns values between 0 and 1

};

#endif