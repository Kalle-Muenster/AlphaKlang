#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H_

//#include <gl/glut.h>;
typedef unsigned GLuint;

#include "Connectable.h"

namespace ProjectMappe
{
	void InitFlatQuat(void);
	void InitICubic(void);
	void InitIBall(void);
	void InitZylinder(void);
	void InitIspheree(void);
	void InitICone(void);
	void InitICapsule(void);
};



		
// IPrimitiv - interface:
template<typename pt>
class IPrimitiv					
{
public:
	pt* primitiv;
	IPrimitiv(void)
		{
			primitiv = new pt();	
			primitiv->component = this;
		}
	virtual ~IPrimitiv(void)
		{
			delete primitiv;
		}
	pt* getPrimitiv(void)
		{
			return (pt*)primitiv;
		}



};

//base class for all Static Primitive types...
//used for switching the primitive on an objects at runtime
struct IlPrimitivo
{
	//static GLuint shape;
	//static GLuint vertsBufferID;
	//static GLuint paintBufferID;
	//static GLuint normsBufferID;
	//static GLuint VertsCount;

public:
	void* component;
	IPrimitiv<IlPrimitivo>* Primitive()
		{
			//shape=0;
			//vertsBufferID=0;
			//paintBufferID=0;
			//normsBufferID=0;
			//VertsCount=0;
			return (IPrimitiv<IlPrimitivo>*)component;
		}
	template<typename P> P* GetPrimitiv(int index = 1000)
		{
			unsigned thc = typeid(P).hash_code();

			if(index!=1000)
				{
					for(int i=0;i<5;i++)
						if(*Primitive[i]->hash_code() == thc)
							return &P
				}
		}
	template<typename P> void SetPrimitiv(void)
		{
			Primitive()->primitiv = new P();
		}
};


// the primitive types themselves:

struct IFlatquad  : public IlPrimitivo
{
	static GLuint shape;
	static GLuint vertsBufferID;
	static GLuint paintBufferID;
	static GLuint normsBufferID;
	static GLuint VertsCount;
};

struct ICubic : public IlPrimitivo
{   
	static GLuint shape;
	static GLuint vertsBufferID;
	static GLuint paintBufferID;
	static GLuint normsBufferID;
	static GLuint VertsCount;
};

struct IZylinder : public IlPrimitivo
{
	static GLuint shape;
	static GLuint vertsBufferID;
	static GLuint paintBufferID;
	static GLuint normsBufferID;
	static GLuint VertsCount;
};

struct IBall : public IlPrimitivo
{
	static GLuint shape;
	static GLuint vertsBufferID;
	static GLuint paintBufferID;
	static GLuint normsBufferID;
	static GLuint VertsCount;
};

struct ISphere : public IlPrimitivo
{
	static GLuint shape;
	static GLuint vertsBufferID;
	static GLuint paintBufferID;
	static GLuint normsBufferID;
	static GLuint VertsCount;
};

struct ICone : public IlPrimitivo
{
	static GLuint shape;
	static GLuint vertsBufferID;
	static GLuint paintBufferID;
	static GLuint normsBufferID;
	static GLuint VertsCount;
};

struct ICapsule	: public IlPrimitivo
{
	static GLuint shape;
	static GLuint vertsBufferID;
	static GLuint paintBufferID;
	static GLuint normsBufferID;
	static GLuint VertsCount;
};


#endif