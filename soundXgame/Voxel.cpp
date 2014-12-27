#include "projectGrafics.h"
#include "projectMacros.h"

#include "VoxelMap.h"
//#include "Voxel.h"


Voxel::Voxel(void)
{
	position.iX=position.Yps=size.ix=size.yps=0;
	x=&position.iX;
	y=&position.Yps;
	w=&size.ix;
	h=&size.yps;
	farb.s32 = 0;
}

void
Voxel::SetParentVoxmap(VoxelMap* map)
{
	vMapObject = map;
}

Voxel::~Voxel(void)
{
//	delete vMapObject;
}

VectorF
Voxel::GetPosition(void)
{
	VectorF neu;
	neu.x =  position.iX * MainDimensions->x;
	neu.y = position.Yps * MainDimensions->x;
	return neu;
}

void
Voxel::SetPosition(VectorF setter)
{
	position = setter/MainDimensions->x; 
}

VectorF
Voxel::GetSize(void)
{
	VectorF neu;
	neu.x = size.ix * MainDimensions->y;
	neu.y = size.yps * MainDimensions->y;
	return neu;
}

void
Voxel::SetSize(VectorF setter)
{
	size.ix = (setter/MainDimensions->y).x; 
	size.yps = (setter/MainDimensions->y).y;
}


void Voxel::SetNeighbours(int ol,int or,int ur,int ul)
{
	OL=ol;
	OR=or;
	UR=ur;
	UL=ul;
}


void Voxel::vDraw(VectorPF offset,char flipt)
{		
	//VectorF offset = *VectorF::Zero;
	//offset.x = *P_offset.x;
	//offset.y = *P_offset.y;
	Vector3 values;
	VectorF sizer;

	farb.s32 = color;	
	glColor4f((GLfloat)farb.Bytss[0]/255,(GLfloat)farb.Bytss[1]/255,(GLfloat)farb.Bytss[2]/255,0.5f);

	GLfloat F=((float)factor/255)+(1.0f-(*offset.y/SCREENHEIGHT)) * *factorPointer;
	switch(flipt)
	{
	case 'z':
		values.x =  *TheOtherZED; 
		values.y = (position.Yps * MainDimensions->y + *offset.y)*F;
		values.z = (position.iX  * MainDimensions->y + *offset.x)*F;
		sizer.x = (size.yps * MainDimensions->x)*F;
		sizer.y = (size.ix  * MainDimensions->x)*F;

		glVertex3f( values.x, values.y			 , values.z );
		glVertex3f( values.x, values.y			 , values.z + sizer.x );
		glVertex3f( values.x, values.y + sizer.y , values.z + sizer.x );
		glVertex3f( values.x, values.y + sizer.y , values.z );
		break;
	case 'y':
		values.x = (position.iX * MainDimensions->y+ *offset.x) *F;
		values.y = *TheOtherZED; 
		values.z = (position.Yps*MainDimensions->y+ *offset.y) *F;
		sizer.x = (size.yps*MainDimensions->x)*F;
		sizer.y = (size.ix*MainDimensions->x)*F;

		glVertex3f( values.x		  , values.y		  ,	values.z );
		glVertex3f( values.x		  , values.y + sizer.x,	values.z );
		glVertex3f( values.x + sizer.y, values.y + sizer.x, values.z );
		glVertex3f( values.x + sizer.y, values.y		  ,	values.z );
		break;
	case 'x':
		values.x = (position.iX * MainDimensions->y+ *offset.x) *F;
		values.y = (position.Yps*MainDimensions->y+ *offset.y) *F;
		values.z = *TheOtherZED;
		sizer.x = (size.yps*MainDimensions->x)*F;
		sizer.y = (size.ix*MainDimensions->x)*F;

		glVertex3f( values.x,			values.y ,			 values.z );
		glVertex3f( values.x + sizer.x,	values.y ,			 values.z );
		glVertex3f( values.x + sizer.x,	values.y + sizer.y,  values.z );
		glVertex3f( values.x,			values.y + sizer.y,	 values.z );
		break;
	}
}



void Voxel::vDrawBunt(VectorPF offset,char flipt)
{

	GLfloat F=((float)factor/255)+(1.0f-(*offset.y/SCREENHEIGHT)) * *factorPointer;
	int l = (int)*x-1;
	l=(l>=0)?l:0;
	int r = (int)*x+1;
	r = (r<vMapObject->mapWidth)?r:vMapObject->mapWidth-1;
	int o = (int)*y-1;
	o=(o>=0)?o:0;
	int u = (int)*y+1;
	u=(u<vMapObject->mapHeight)?u:vMapObject->mapHeight-1;  

	if(flipt=='z')
	{
		farb.s32=OL;
		glColor4f((GLfloat)farb.Bytss[0]/255,(GLfloat)farb.Bytss[1]/255,(GLfloat)farb.Bytss[2]/255,0.5f);
		glVertex3f(*TheOtherZED, (position.Yps*MainDimensions->y + *offset.y)*F									 , (position.iX * MainDimensions->y + *offset.x) *F );
		farb.s32=OR;
		glColor4f((GLfloat)farb.Bytss[0]/255,(GLfloat)farb.Bytss[1]/255,(GLfloat)farb.Bytss[2]/255,0.5f);
		glVertex3f(*TheOtherZED, (position.Yps*MainDimensions->y + *offset.y)*F									 , (position.iX * MainDimensions->y + *offset.x) *F + (size.ix*MainDimensions->x)*F);
		farb.s32=UR;
		glColor4f((GLfloat)farb.Bytss[0]/255,(GLfloat)farb.Bytss[1]/255,(GLfloat)farb.Bytss[2]/255,0.5f);
		glVertex3f(*TheOtherZED, (position.Yps*MainDimensions->y + *offset.y)*F + (size.yps*MainDimensions->x)*F , (position.iX * MainDimensions->y + *offset.x) *F + (size.ix*MainDimensions->x)*F);
		farb.s32=UL;
		glColor4f((GLfloat)farb.Bytss[0]/255,(GLfloat)farb.Bytss[1]/255,(GLfloat)farb.Bytss[2]/255,0.5f);
		glVertex3f(*TheOtherZED, (position.Yps*MainDimensions->y + *offset.y)*F + (size.yps*MainDimensions->x)*F , (position.iX * MainDimensions->y + *offset.x) *F );
	} 
	else
	{
		farb.s32=OL;
		glColor4f((GLfloat)farb.Bytss[0]/255,(GLfloat)farb.Bytss[1]/255,(GLfloat)farb.Bytss[2]/255,0.5f);
		glVertex3f((position.iX*MainDimensions->y+*offset.x)*F,								(position.Yps*MainDimensions->y+*offset.y)*F, *TheOtherZED);
		
		farb.s32=OR;
		glColor4f((GLfloat)farb.Bytss[0]/255,(GLfloat)farb.Bytss[1]/255,(GLfloat)farb.Bytss[2]/255,0.5f);
		glVertex3f((position.iX*MainDimensions->y+*offset.x)*F + (size.ix*MainDimensions->x)*F ,(position.Yps*MainDimensions->y+*offset.y)*F, *TheOtherZED);
		
		farb.s32=UR;
		glColor4f((GLfloat)farb.Bytss[0]/255,(GLfloat)farb.Bytss[1]/255,(GLfloat)farb.Bytss[2]/255,0.5f);
		glVertex3f((position.iX*MainDimensions->y+*offset.x)*F + (size.ix*MainDimensions->x)*F ,(position.Yps*MainDimensions->y+*offset.y)*F + (size.yps*MainDimensions->x)*F, *TheOtherZED);
		
		farb.s32=UL;
		glColor4f((GLfloat)farb.Bytss[0]/255,(GLfloat)farb.Bytss[1]/255,(GLfloat)farb.Bytss[2]/255,0.5f);
		glVertex3f((position.iX*MainDimensions->y+*offset.x)*F,								(position.Yps*MainDimensions->y+*offset.y)*F  + (size.yps*MainDimensions->x)*F, *TheOtherZED);
	}


}



void Voxel::vDrawByte(VectorPF offset,char flipt)
{

	GLfloat F = ((float)factor/255)+(1.0f-(*offset.y/SCREENHEIGHT)) * *factorPointer;

	if(flipt=='z')
	{
		farb.s32=OL;
		glColor4b(farb.Bytss[0],farb.Bytss[1],farb.Bytss[2],farb.Bytss[3]);
		glVertex3f(*TheOtherZED, (position.Yps*MainDimensions->y+*offset.y)*F								  ,	(position.iX * MainDimensions->y+*offset.x) *F );
		farb.s32=OR;
		glColor4b(farb.Bytss[0],farb.Bytss[1],farb.Bytss[2],farb.Bytss[3]);
		glVertex3f(*TheOtherZED, (position.Yps*MainDimensions->y+*offset.y)*F								  , (position.iX * MainDimensions->y+*offset.x) *F + (size.ix*MainDimensions->x)*F);
		farb.s32=UR;
		glColor4b(farb.Bytss[0],farb.Bytss[1],farb.Bytss[2],farb.Bytss[3]);
		glVertex3f(*TheOtherZED, (position.Yps*MainDimensions->y+*offset.y)*F + (size.yps*MainDimensions->x)*F ,	(position.iX * MainDimensions->y+*offset.x) *F + (size.ix*MainDimensions->x)*F);
		farb.s32=UL;
		glColor4b(farb.Bytss[0],farb.Bytss[1],farb.Bytss[2],farb.Bytss[3]);
		glVertex3f(*TheOtherZED, (position.Yps*MainDimensions->y+*offset.y)*F + (size.yps*MainDimensions->x)*F ,	(position.iX * MainDimensions->y+*offset.x) *F );
	} 
	else
	{
		farb.s32=OL;
		glColor4b(farb.Bytss[0],farb.Bytss[1],farb.Bytss[2],farb.Bytss[3]);
		glVertex3f((position.iX*MainDimensions->y+*offset.x)*F,								(position.Yps*MainDimensions->y+*offset.y)*F, *TheOtherZED);
		
		farb.s32=OR;
		glColor4b(farb.Bytss[0],farb.Bytss[1],farb.Bytss[2],farb.Bytss[3]);
		glVertex3f((position.iX*MainDimensions->y+*offset.x)*F + (size.ix*MainDimensions->x)*F ,(position.Yps*MainDimensions->y+*offset.y)*F, *TheOtherZED);
		
		farb.s32=UR;
		glColor4b(farb.Bytss[0],farb.Bytss[1],farb.Bytss[2],farb.Bytss[3]);
		glVertex3f((position.iX*MainDimensions->y+*offset.x)*F + (size.ix*MainDimensions->x)*F ,(position.Yps*MainDimensions->y+*offset.y)*F + (size.yps*MainDimensions->x)*F, *TheOtherZED);
		
		farb.s32=UL;
		glColor4b(farb.Bytss[0],farb.Bytss[1],farb.Bytss[2],farb.Bytss[3]);
		glVertex3f((position.iX*MainDimensions->y+*offset.x)*F,								(position.Yps*MainDimensions->y+*offset.y)*F  + (size.yps*MainDimensions->x)*F, *TheOtherZED);
	}


}