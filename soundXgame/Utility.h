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




typedef u32_2s16_4b8* Color;

class Loader
{
private:
	unsigned _w,_h,_length;
	std::vector<u32_2s16_4b8> _Data;
public:
	Color data;
	Loader(void);
	Loader(const char* filename);
	~Loader(void);
	int width(int=NULL);
	int height(int=NULL);
	int count(void);
	Color Pixel(int);
	Color Pixel(int,int);
	void* LoadeFile(const char* filename);
};

class Utility 
{
private:
	Utility();
	virtual ~Utility();
public:
	static void loadObj(const char*,  std::vector<glm::vec3> &, std::vector<glm::vec2> &, std::vector<glm::vec3> &);
	static GLuint loadTexture(const char *);
//	static Vector3 LoadePPMBitmap(const char* filename,Voxel* buffer);
};

#endif