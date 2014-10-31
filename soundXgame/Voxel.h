#ifndef __DING__
#define __DING__


#include "Vector.h"
//#include "VoxelMap.h"
//#include "Connectable.h"
//#include "projectMacros.h"


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
	void fDrawBlure(VectorF offset);
	void SetNeighbours(int ol,int or,int ur,int ul);
//	__declspec(property(get=GetPosition,put=SetPosition))VectorF position;
//	__declspec(property(get=GetSize,put=SetSize))VectorF size;
	VectorF GetPosition(void); 
	void SetPosition(VectorF setter);
	VectorF GetSize(void); 
	void SetSize(VectorF setter);
	
private:
	int OL;
	int OR;
	int UR;
	int UL;
};





#endif

