#ifndef __UTILITY_H__
#define __UTILITY_H__


#include "projectGrafics.h"
#include <bass.h>
#include <bass_fx.h>
#include <vector>
#include <iostream>
#include "Voxel.h"

struct Vector3 : public glm::vec3
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

struct Transform 
{
	
	glm::vec3 position;
	glm::vec3 scale;
	glm::vec3 rotation;

	glm::vec3 forward,right,up;
};

struct TransformA
{
	
	Vector3 position;
	Vector3 scale;
	BASS_3DVECTOR movement;
	Vector3 rotation;

	Vector3 forward,right,up;
};

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


//struct ScanLine
//{
//  u32_2s16_4b8 *data;
//};
//struct ScanLine_64 : public ScanLine
//{
//private:
//	u32_2s16_4b8 Data[64];
//public:
//	ScanLine_64(void)
//	{
//		data=&Data[0];
//	}
//};
//struct ScanLine_128 : public ScanLine
//{
//private:
//	u32_2s16_4b8 Data[128];
//public:
//	ScanLine_128(void)
//	{
//		data=&Data[0];
//	}
//};
//struct ScanLine_256 : public ScanLine
//{
//private:
//	u32_2s16_4b8 Data[256];
//public:
//	ScanLine_256(void)
//	{
//		data=&Data[0];
//	}
//};
//
//
//struct _BitMap_
//{
//	POWERS_OF_TWO width,height;
//	ScanLine *data;
//};
//struct BitMap_64 : _BitMap_ 
//{
//private:
//	ScanLine Data[64]; 
//public:
//	BitMap_64(void)
//	{
//		data = &Data[0];
//	}
//};
//struct BitMap_128 : _BitMap_ 
//{
//private:
//	ScanLine Data[128]; 
//public:
//	BitMap_128(void)
//	{
//		data = &Data[0];
//	}
//};
//struct BitMap_256 : _BitMap_ 
//{
//private:
//	ScanLine Data[256]; 
//public:
//	BitMap_256(void)
//	{
//		data = &Data[0];
//	} 
//};
//

//
//class BitMap
//{
//private:
//	_BitMap_ _bitmap;
//	unsigned sizeInByte;
//public:
//	//Creates an Object with the given width and height...
//	//width and height each must be PowerOf2 but don't need to be same...
//	BitMap(POWERS_OF_TWO,POWERS_OF_TWO);
//	~BitMap(void);
//	void LoadeFromPPM(const char* filename);
//	unsigned GetSizeInByte(void);
//	//Get or Set the Width in pixel...  !!! Set will discard all previously contained data...!!!!  
//	unsigned short Width(POWERS_OF_TWO = _);
//	//Get or Set the Hight in pixel...  !!! Set will discard all previously contained data...!!!! 
//	unsigned short Height(POWERS_OF_TWO = _);
//	//Returns Color-Values at given Coordinates...
//	Color Pixel(unsigned short x,unsigned short y,int=NULL);
//	//Returns Color-Values at given Index...
//	Color Pixel(int index);
//	//Should return a Pointer to the Object's data... ?...
//	void* GetData(int offset = 0,int sizeByte =NULL);
//
//};

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