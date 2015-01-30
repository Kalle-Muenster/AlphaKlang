#include "IPrimitive.h"
#include "Utility.h"
#include <vector>
#include <glm.hpp>

typedef GLfloat dreier[3];
typedef GLfloat zweier[2];

GLuint
IFlatquad::vertsBufferID=0; 

GLuint
IFlatquad::paintBufferID=0;

GLuint
IFlatquad::normsBufferID=0;

GLuint
IFlatquad::shape = 0;

GLuint 
IFlatquad::VertsCount=0;


void
ProjectMappe::InitFlatQuat(void)
{
	dreier vertsTemp[4];
	zweier paintTemp[4];
	dreier normsTemp[1];

	IFlatquad::shape = GL_QUADS;
	IFlatquad::VertsCount = 4;

	vertsTemp[0][0] = -0.5f;
	vertsTemp[0][1] = -0.5f;
	vertsTemp[0][2] = 0;
	paintTemp[0][0] = 0;
	paintTemp[0][1] = 0;

	vertsTemp[1][0] = 0.5f;
	vertsTemp[1][1] = -0.5f;
	vertsTemp[1][2] = 0;
	paintTemp[1][0] = 1;
	paintTemp[1][1] = 0;

	vertsTemp[2][0] = 0.5f;
	vertsTemp[2][1] = 0.5f;
	vertsTemp[2][2] = 0;
	paintTemp[2][0] = 1;
	paintTemp[2][1] = 1;

	vertsTemp[3][0] = -0.5f;
	vertsTemp[3][1] = 0.5f;
	vertsTemp[3][2] = 0;
	paintTemp[3][0] = 0;
	paintTemp[3][1] = 1;

	normsTemp[0][0] = 0;
	normsTemp[0][1] = 0;
	normsTemp[0][2] = 1;

	glGenBuffers(1, &IFlatquad::vertsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, IFlatquad::vertsBufferID);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(dreier), &vertsTemp[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IFlatquad::paintBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, IFlatquad::paintBufferID);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(zweier), &paintTemp[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IFlatquad::normsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, IFlatquad::normsBufferID);
	glBufferData(GL_ARRAY_BUFFER, 1 * sizeof(dreier), &normsTemp[0], GL_STATIC_DRAW);
}


GLuint 
ICubic::vertsBufferID=0;

GLuint
ICubic::paintBufferID=0;

GLuint
ICubic::normsBufferID=0;

GLuint
ICubic::shape = 0;

GLuint
ICubic::VertsCount = 0; 

void
ProjectMappe::InitICubic(void)
{
	std::vector<glm::vec3> vertsTemp;
	std::vector<glm::vec2> paintTemp;
	std::vector<glm::vec3> normsTemp;


	Utility::loadObj("cube_quads.obi",vertsTemp,paintTemp,normsTemp,ICubic::shape);

	glGenBuffers(1, &ICubic::vertsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, ICubic::vertsBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertsTemp.size() * sizeof(glm::vec3), &vertsTemp[0], GL_STATIC_DRAW);
	ICubic::VertsCount = vertsTemp.size();

	glGenBuffers(1, &ICubic::paintBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, ICubic::paintBufferID);
	glBufferData(GL_ARRAY_BUFFER, paintTemp.size() * sizeof(glm::vec2), &paintTemp[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ICubic::normsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, ICubic::normsBufferID);
	glBufferData(GL_ARRAY_BUFFER, normsTemp.size() * sizeof(glm::vec3), &normsTemp[0], GL_STATIC_DRAW);

}


GLuint
IBall::vertsBufferID=0;
GLuint
IBall::paintBufferID=0;
GLuint
IBall::normsBufferID=0;
GLuint
IBall::shape = 0;
GLuint
IBall::VertsCount = 0; 

void
ProjectMappe::InitIBall(void)
{
	std::vector<glm::vec3> vertsTemp;
	std::vector<glm::vec2> paintTemp;
	std::vector<glm::vec3> normsTemp;

	Utility::loadObj("sphere_tris_low.obi",vertsTemp,paintTemp,normsTemp,IBall::shape);

	glGenBuffers(1, &IBall::vertsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, IBall::vertsBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertsTemp.size() * sizeof(glm::vec3), &vertsTemp[0], GL_STATIC_DRAW);
	IBall::VertsCount = vertsTemp.size();

	glGenBuffers(1, &IBall::paintBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, IBall::paintBufferID);
	glBufferData(GL_ARRAY_BUFFER, paintTemp.size() * sizeof(glm::vec2), &paintTemp[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IBall::normsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, IBall::normsBufferID);
	glBufferData(GL_ARRAY_BUFFER, normsTemp.size() * sizeof(glm::vec3), &normsTemp[0], GL_STATIC_DRAW);
}


GLuint
	IZylinder::vertsBufferID=0;
GLuint
	IZylinder::paintBufferID=0;
GLuint
	IZylinder::normsBufferID=0;
GLuint
	IZylinder::shape=0;
GLuint
	IZylinder::VertsCount = 0; 

void
ProjectMappe::InitZylinder(void)
{
	std::vector<glm::vec3> vertsTemp;
	std::vector<glm::vec2> paintTemp;
	std::vector<glm::vec3> normsTemp;


	Utility::loadObj("cylinder_quads.obi",vertsTemp,paintTemp,normsTemp,IZylinder::shape);

	glGenBuffers(1, &IZylinder::vertsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, IZylinder::vertsBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertsTemp.size() * sizeof(glm::vec3), &vertsTemp[0], GL_STATIC_DRAW);
	IZylinder::VertsCount = vertsTemp.size();

	glGenBuffers(1, &IZylinder::paintBufferID);
	glBindBuffer(GL_ARRAY_BUFFER,IZylinder::paintBufferID);
	glBufferData(GL_ARRAY_BUFFER, paintTemp.size() * sizeof(glm::vec2), &paintTemp[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IZylinder::normsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, IZylinder::normsBufferID);
	glBufferData(GL_ARRAY_BUFFER, normsTemp.size() * sizeof(glm::vec3), &normsTemp[0], GL_STATIC_DRAW);
}

GLuint
	ISphere::vertsBufferID=0;
GLuint
	ISphere::paintBufferID=0;
GLuint
	ISphere::normsBufferID=0;
GLuint
	ISphere::shape = 0;
GLuint
	ISphere::VertsCount = 0; 

void
ProjectMappe::InitIspheree(void)
{
	std::vector<glm::vec3> vertsTemp;
	std::vector<glm::vec2> paintTemp;
	std::vector<glm::vec3> normsTemp;
	GLuint shape;

	Utility::loadObj("sphere_quads_low.obi",vertsTemp,paintTemp,normsTemp,ISphere::shape);

	glGenBuffers(1, &ISphere::vertsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, ISphere::vertsBufferID);
	glBufferData(GL_ARRAY_BUFFER, vertsTemp.size() * sizeof(glm::vec3), &vertsTemp[0], GL_STATIC_DRAW);
	ISphere::VertsCount = vertsTemp.size();

	glGenBuffers(1, &ISphere::paintBufferID);
	glBindBuffer(GL_ARRAY_BUFFER,ISphere::paintBufferID);
	glBufferData(GL_ARRAY_BUFFER, paintTemp.size() * sizeof(glm::vec2), &paintTemp[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ISphere::normsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, ISphere::normsBufferID);
	glBufferData(GL_ARRAY_BUFFER, normsTemp.size() * sizeof(glm::vec3), &normsTemp[0], GL_STATIC_DRAW);
}