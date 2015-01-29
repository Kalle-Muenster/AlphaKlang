#ifndef __VECTOR__F
#define __VECTOR__F

typedef int BOOL;

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
	VectorF operator +=(VectorF other);
	VectorF cros(VectorF other);
	bool operator ==(VectorF other);
	bool operator !=(VectorF other);
	bool operator <(VectorF other);
	bool operator >(VectorF other);
	static const VectorF* const Zero;
	VectorF(void);
	VectorF(float,float);
};
	
struct VectorPF
{
public:
	float *x;
	float *y;

	operator VectorI();
	operator VectorF();
	void operator =(VectorF);
	VectorF operator +(VectorPF);
	VectorF operator -(VectorPF);
	VectorF operator *(float s);
	VectorF operator /(float s);
	static const VectorPF* const Zero;
};

namespace ProjectMappe
{
	class Rectangle
	{
	private:
		VectorPF Center;
		VectorPF HalbSize;
	public:
		Rectangle(void);
		Rectangle(float,float,float,float);
		Rectangle(float*cX,float*cY,float*hW,float*hH);
		VectorF GetPosition(void);
		void SetPosition(float,float);
		void SetPosition(VectorF);
		VectorF GetSize(void);
		void SetSize(float,float);
		void SetSize(VectorF);
		VectorF GetHalbSize(void);
		VectorF GetCenter(void);
		bool Containes(VectorF);

		static const Rectangle *const Zero;
	};

	void GlobalDestructor(void);
	void StartupRuntimeManagers(void);
	bool EXIT(BOOL=3);
};


//#define Rectangle ProjectMappe::Rectangle

#endif