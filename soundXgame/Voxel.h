#ifndef __DING__
#define __DING__

#include "VoxelVector.h"


 class Voxel
{
public:
	Voxel(void);
	~Voxel(void);
	
union 
	{
	int s32;
	unsigned char Bytss[4];
	}farb;
	GLfloat factor;
	int color;
	VectorI position;
	Vecti size;
	GLint  *x,*y;
	GLshort *w,*h;
    VectorF* MainDimensions;
	void pDraw(void);
	void fDraw(VectorPF offset);
	void fDrawBlure(VectorPF offset);
	void SetNeighbours(int ol,int or,int ur,int ul);
	VectorF GetPosition(void); 
	void SetPosition(VectorF setter);
	VectorF GetSize(void); 
	void SetSize(VectorF setter);
	GLfloat* TheOtherZED;
private:
	int OL;
	int OR;
	int UR;
	int UL;
};





#endif

