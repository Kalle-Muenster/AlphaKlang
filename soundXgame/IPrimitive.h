#ifndef _PRIMITIVE_H_
#define _PRIMITIVE_H_

//#include <gl/glut.h>;
typedef unsigned GLuint;

namespace ProjectMappe
{
	void InitFlatQuat(void);
	void InitICubic(void);
	void InitIBall(void);
	void InitZylinder(void);
	void InitIspheree(void);
};

template<typename pType>
struct IPrimitive : public pType
{
 	
};



struct IFlatquad 
{
		static GLuint shape;
	static GLuint vertsBufferID;
	static GLuint paintBufferID;
	static GLuint normsBufferID;
	static GLuint VertsCount;
};

struct ICubic
{   
		static GLuint shape;
	static GLuint vertsBufferID;
	static GLuint paintBufferID;
	static GLuint normsBufferID;
	static GLuint VertsCount;
};

struct IZylinder
{
		static GLuint shape;
	static GLuint vertsBufferID;
	static GLuint paintBufferID;
	static GLuint normsBufferID;
	static GLuint VertsCount;
};

struct IBall
{
		static GLuint shape;
	static GLuint vertsBufferID;
	static GLuint paintBufferID;
	static GLuint normsBufferID;
	static GLuint VertsCount;
};

struct ISphere
{
		static GLuint shape;
	static GLuint vertsBufferID;
	static GLuint paintBufferID;
	static GLuint normsBufferID;
	static GLuint VertsCount;
};
#endif