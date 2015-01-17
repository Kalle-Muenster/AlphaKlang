#ifndef __DINGERMAP__
#define __DINGERMAP__


#include <stdio.h>
#include <istream>
#include "VoxelVector.h"
#include "Voxel.h"

//class Voxel;

class VoxelMap
{
public:

	VoxelMap(void);
	~VoxelMap(void);
	unsigned numberOfVoxelers;
	unsigned mapWidth,mapHeight;
	VectorPF position;
	VectorF size;
	float voxelSize;
	VectorF MainSizzes;
	GLfloat BumpFactor;
	char flipt;
	void Loade(char* filename,void*);
	void ReLoade(char* filename);
	void LoadeMap(char* filename,int channel);
	
	void Draw(VectorPF position);
	void DrawBunt(VectorPF position);
	void DrawByte(VectorPF position);

	float loadScale;
	void* SetVoxelerBuffer(Voxel* voxels);
	Voxel* GetVoxel(int x,int y);
	Voxel* GetVoxel(int index);
	Voxel* voxels;
	

	
	bool loaded;
};





#endif

