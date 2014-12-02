#ifndef __VECTOR__F
#define __VECTOR__F

//#include "DataStructs.h"

class VoxtructsInitiator
{
public:
	static void initiateVoxtructs(void);
};

 struct VectorI
{
public:
	int iX;
	int Yps;
	VectorI operator *(float s);
	static const VectorI* const Zero;
};

 struct Vecti
{
public:
	signed short ix;
	signed short yps;
	Vecti operator *(float s);
	static const Vecti* const Zero;
};

 struct VectorF
{
public:
	float x,y;
	operator VectorI();
	VectorF operator +(VectorF other);
	VectorF operator -(VectorF subtractor);
	VectorF operator *(float s);
	VectorF operator /(float s);
	VectorF cros(VectorF other);
	bool operator ==(VectorF other);
	bool operator !=(VectorF other);
	static const VectorF* const Zero;
};
	
struct VectorPF
{
public:
	float *x;
	float *y;

	operator VectorI();
	VectorPF operator *(float s);
	VectorPF operator /(float s);
	static const VectorPF* const Zero;
};

namespace ProjectMappe
{
	class Rectangle
	{
	private:
		VectorF Position;
		VectorF HalbSize;
	public:
		Rectangle(void);
		Rectangle(float,float,float,float);
		VectorF position(void);
		VectorF position(float,float);
		VectorF position(VectorF);
		VectorF size(void);
		VectorF size(float,float);
		VectorF size(VectorF);
		VectorF center(void);
		static const Rectangle *const Zero;
	};
}

//#define Rectangle ProjectMappe::Rectangle

#endif