#ifndef __DING__
#define __DING__

//#include "VoxelVector.h"
#include "DataStructs.h"

class VoxelMap;

 class Voxel
{
public:

	Voxel(void);
	~Voxel(void);
	
  union 
	{
	int s32;
	unsigned char Bytss[4];
	} farb;
	GLfloat factor;
	GLfloat* factorPointer;
	int color;
	VectorI position;
	Vecti size;
	GLint  *x,*y;
	GLshort *w,*h;
    VectorF* MainDimensions;
	void SetNeighbours(int ol,int or,int ur,int ul);
	VectorF GetPosition(void); 
	void SetPosition(VectorF setter);
	VectorF GetSize(void); 
	void SetSize(VectorF setter);
	GLfloat* TheOtherZED;
	void SetParentVoxmap(VoxelMap* map);
	void vDraw(VectorPF offset);
	void vDrawBunt(VectorPF offset);
	void vDrawByte(VectorPF offset);

private:
	int OL;
	int OR;
	int UR;
	int UL;
	VoxelMap* vMapObject;
};

#endif

