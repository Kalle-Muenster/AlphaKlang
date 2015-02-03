#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H_

//#include <gl/glut.h>;
typedef unsigned GLuint;

#include "Connectable.h"

struct IFlatquad;
struct ICubic;
struct IBall;
struct ISphere;
struct IZylinder;
struct ICone;
struct ICapsule;


struct PrimitivMesh	
{
	GLuint shape;
	GLuint vertsBufferID;
	GLuint paintBufferID;
	GLuint normsBufferID;
	GLuint VertsCount;
};

class PrimitiveType
{
public:
	enum PRIMITIVE : char
	{FLATQUAD,QUBE,BALL,SPHERE,ZYLINDER,CONE,CAPSULA};
	PrimitivMesh buffers;
	static const IFlatquad * FlatQuad;
	const static ICubic * Qube;
	const static IBall * Ball;
	const static ISphere * Sphere;
	const static IZylinder * Zylinder;
	const static ICone * Cone;
	const static ICapsule * Capsula;
};

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
//template<typename pType>
//class IPrimitiv					
//{
//public:
//	pType* primitiv;
//	IPrimitiv(void)
//		{
//			primitiv = new pType();	
//			primitiv->component = this;
//		}
//	virtual ~IPrimitiv(void)
//		{
//			delete primitiv;
//		}
//	virtual operator pType(void)
//	{
//		return *primitiv;
//	}
//};

//base class for all Static Primitive types...
//used for switching the primitive on an objects at runtime
//struct PrimitivType	
//
//
//public:
//	void* component;
//	IPrimitiv<PrimitivType>* Primitive()
//		{
//			return (IPrimitiv<PrimitivType>*)component;
//		}
//	template<typename P> P* GetPrimitiv(int index = 1000)
//		{
//			unsigned thc = typeid(P).hash_code();
//
//			if(index!=1000)
//				{
//					for(int i=0;i<5;i++)
//						if(*Primitive[i]->hash_code() == thc)
//							return &P
//				}
//		}
//	template<typename P> void SetPrimitiv(void)
//		{
//			Primitive()->primitiv = new P();
//		}
//};


// the primitive types themselves:


struct IFlatquad : public PrimitiveType
{
	static const GLuint shape;
	static GLuint vertsBufferID;
	static GLuint paintBufferID;
	static GLuint normsBufferID;
	static const GLuint VertsCount;
};

struct ICubic : public PrimitiveType
{   
	static GLuint shape;
	static GLuint vertsBufferID;
	static GLuint paintBufferID;
	static GLuint normsBufferID;
	static GLuint VertsCount;
};

struct IZylinder : public PrimitiveType
{
	static const GLuint shape;
	static GLuint vertsBufferID;
	static GLuint paintBufferID;
	static GLuint normsBufferID;
	static GLuint VertsCount;
};

struct IBall : public PrimitiveType
{
	static const GLuint shape;
	static GLuint vertsBufferID;
	static GLuint paintBufferID;
	static GLuint normsBufferID;
	static GLuint VertsCount;
};

struct ISphere : public PrimitiveType
{
	static const GLuint shape;
	static GLuint vertsBufferID;
	static GLuint paintBufferID;
	static GLuint normsBufferID;
	static GLuint VertsCount;
};

struct ICone : public PrimitiveType
{
	static const GLuint shape;
	static GLuint vertsBufferID;
	static GLuint paintBufferID;
	static GLuint normsBufferID;
	static GLuint VertsCount;
};

struct ICapsule	: public PrimitiveType
{
	static const GLuint shape;
	static GLuint vertsBufferID;
	static GLuint paintBufferID;
	static GLuint normsBufferID;
	static GLuint VertsCount;
};


#endif