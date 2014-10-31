#include "Vector.h"


VectorI
VectorI::operator  *(GLfloat s)
	{
		VectorI neu;
		neu.iX = iX*=s;
		neu.Yps = Yps*=s;
		return neu;
	}

Vecti
Vecti::operator  *(GLfloat s)
	{
		Vecti neu;
		neu.ix = ix*=s;
		neu.yps = yps*=s;
		return neu;
	}

VectorF::operator VectorI()
	{
		VectorI newVector;
		newVector.iX=x;
		newVector.Yps=y;
		return newVector;
	}

VectorF
VectorF::operator *(GLfloat s)
	{
		VectorF newVector;
		newVector.x=x*s;
		newVector.y=y*s;
		return newVector;
	}

VectorF
VectorF::operator /(GLfloat s)
	{
		VectorF newVector;
		newVector.x=x/s;
		newVector.y=y/s;
		return newVector;
	}

VectorPF::operator VectorI()
	{
		VectorI newVector;
		newVector.iX = *x;
		newVector.Yps = *y;
		return newVector;
	}

VectorPF
VectorPF::operator *(GLfloat s)
	{
		VectorPF newVector;
		*newVector.x = *x *s;
		*newVector.y = *y *s;
		return newVector;
	}

VectorPF
VectorPF::operator /(GLfloat s)
	{
		VectorPF newVector;
		*newVector.x=*x/s;
		*newVector.y=*y/s;
		return newVector;
	}