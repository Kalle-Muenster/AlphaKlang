#ifndef __VECTOR__F
#define __VECTOR__F

#include <GL\freeglut.h>


 struct VectorI
{
public:
	GLint iX;
	GLint Yps;
	VectorI operator *(GLfloat s);
};

 struct Vecti
{
public:
	signed short ix;
	signed short yps;
	Vecti operator *(GLfloat s);
};

 struct VectorF
{
public:
	GLfloat x,y;
	operator VectorI();
	VectorF operator *(GLfloat s);
	VectorF operator /(GLfloat s);
};
	
struct VectorPF
{
public:
	GLfloat *x;
	GLfloat *y;

	operator VectorI();
	VectorPF operator *(GLfloat s);
	VectorPF operator /(GLfloat s);
};

#endif