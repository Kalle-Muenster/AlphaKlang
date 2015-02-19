#include "IPrimitive.h"
#include "Utility.h"
#include <vector>
#include <glm.hpp>

//Very simple vectors...
typedef GLfloat dreier[3];
typedef GLfloat zweier[2];



//A Type-Array, containing constants of the primitives Type-ID's
const type_info* _primitiveTypes[8] = { &(typeid(IFlatquad)),
									    &(typeid(ICubic)),
									    &(typeid(IBall)),
									    &(typeid(ISphere)),
									    &(typeid(IZylinder)),
									    &(typeid(ICone)),
									    &(typeid(ICapsule)),
										&(typeid(IPyramidal)) };

PrimitiveType* _primitivs[8] = { new IFlatquad(),
								 new ICubic(),
								 new IBall(),
								 new ISphere(),
								 new IZylinder(),
								 new ICone(),
								 new ICapsule(),
								 new IPyramidal() };

IFlatquad	_flatquad	= IFlatquad();
ICubic		_qube		= ICubic();
IBall		_ball		= IBall();
ISphere		_sphere		= ISphere();
IZylinder	_zylinder	= IZylinder();
ICone		_cone		= ICone();
ICapsule	_capsula	= ICapsule();


const IFlatquad const *
	PrimitiveType::FlatQuad = (IFlatquad const*) _primitivs[PRIMITIVE::FLATQUAD]; 

const ICubic const *
	PrimitiveType::Qube = (ICubic const*) _primitivs[PRIMITIVE::QUBE];

const IBall const *
	PrimitiveType::Ball = (IBall const*) _primitivs[PRIMITIVE::BALL];

const ISphere const *
	PrimitiveType::Sphere = (ISphere const*) _primitivs[PRIMITIVE::SPHERE];

const IZylinder const *
	PrimitiveType::Zylinder = (IZylinder const*) _primitivs[PRIMITIVE::ZYLINDER];

const ICone const *
	PrimitiveType::Cone = (ICone const*) _primitivs[PRIMITIVE::CONE];

const ICapsule const *
	PrimitiveType::Capsula = (ICapsule const*) _primitivs[PRIMITIVE::CAPSULA];

const IPyramidal const *
	PrimitiveType::Pyramid = (IPyramidal const*) _primitivs[PRIMITIVE::PYRAMID]; 

//////////////////////////////////////////////////////////////
// Flatquad static definitions:
//////////////////////////////////////////////////////////////


const GLuint
	IFlatquad::shape = GL_QUADS;
const GLuint 
	IFlatquad::VertsCount = 4;
GLuint
	IFlatquad::vertsBufferID = 0; 
GLuint
	IFlatquad::paintBufferID = 0;
GLuint
	IFlatquad::normsBufferID = 0;



// Initiation-function for IFlatquad-static's
void
ProjectMappe::InitFlatQuat(void)
{
	dreier vertsTemp[IFlatquad::VertsCount];
	zweier paintTemp[IFlatquad::VertsCount];
	dreier normsTemp[1];

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
	glBufferData(GL_ARRAY_BUFFER, IFlatquad::VertsCount * sizeof(dreier), &vertsTemp[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IFlatquad::paintBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, IFlatquad::paintBufferID);
	glBufferData(GL_ARRAY_BUFFER, IFlatquad::VertsCount * sizeof(zweier), &paintTemp[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IFlatquad::normsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, IFlatquad::normsBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(dreier), &normsTemp[0], GL_STATIC_DRAW);

	_primitivs[PrimitiveType::PRIMITIVE::FLATQUAD]->buffers.vertsBufferID = IFlatquad::vertsBufferID;
	_primitivs[PrimitiveType::PRIMITIVE::FLATQUAD]->buffers.paintBufferID = IFlatquad::paintBufferID;
	_primitivs[PrimitiveType::PRIMITIVE::FLATQUAD]->buffers.normsBufferID = IFlatquad::normsBufferID;
	_primitivs[PrimitiveType::PRIMITIVE::FLATQUAD]->buffers.VertsCount	   = IFlatquad::VertsCount;
	_primitivs[PrimitiveType::PRIMITIVE::FLATQUAD]->buffers.shape		   = IFlatquad::shape;

	_flatquad.vertsBufferID = IFlatquad::vertsBufferID;
	_flatquad.paintBufferID = IFlatquad::paintBufferID;
	_flatquad.normsBufferID = IFlatquad::normsBufferID;
}



/////////////////////////////////////////////////////////////////////////////////////////
//CUBE:
/////////////////////////////////////////////////////////////////////////////////////////

GLuint
	ICubic::shape = 0;
GLuint
	ICubic::VertsCount = 0;
GLuint 
	ICubic::vertsBufferID=0;
GLuint
	ICubic::paintBufferID=0;
GLuint
	ICubic::normsBufferID=0;
 


void
ProjectMappe::InitICubic(void)
{
	std::vector<glm::vec3> vertsTemp;
	std::vector<glm::vec2> paintTemp;
	std::vector<glm::vec3> normsTemp;

	Utility::loadObj("cube_quads.obi",vertsTemp,paintTemp,normsTemp,ICubic::shape);
	ICubic::VertsCount = vertsTemp.size();

	glGenBuffers(1, &ICubic::vertsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, ICubic::vertsBufferID);
	glBufferData(GL_ARRAY_BUFFER, ICubic::VertsCount * sizeof(glm::vec3), &vertsTemp[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ICubic::paintBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, ICubic::paintBufferID);
	glBufferData(GL_ARRAY_BUFFER, paintTemp.size() * sizeof(glm::vec2), &paintTemp[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ICubic::normsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, ICubic::normsBufferID);
	glBufferData(GL_ARRAY_BUFFER, normsTemp.size() * sizeof(glm::vec3), &normsTemp[0], GL_STATIC_DRAW);

	_primitivs[PrimitiveType::PRIMITIVE::QUBE]->buffers.vertsBufferID = ICubic::vertsBufferID;
	_primitivs[PrimitiveType::PRIMITIVE::QUBE]->buffers.paintBufferID = ICubic::paintBufferID;
	_primitivs[PrimitiveType::PRIMITIVE::QUBE]->buffers.normsBufferID = ICubic::normsBufferID;
	_primitivs[PrimitiveType::PRIMITIVE::QUBE]->buffers.VertsCount    = ICubic::VertsCount;

	_qube.vertsBufferID = ICubic::vertsBufferID;
	_qube.paintBufferID = ICubic::paintBufferID;
	_qube.normsBufferID = ICubic::normsBufferID;
	_qube.VertsCount    = ICubic::VertsCount;
	_qube.shape		    = ICubic::shape;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// BALL (a sphere, build up by triangular faces(like some footballs are...))
///////////////////////////////////////////////////////////////////////////////////////////////////////////

const GLuint
	IBall::shape = GL_TRIANGLES;
GLuint
	IBall::vertsBufferID=0;
GLuint
	IBall::paintBufferID=0;
GLuint
	IBall::normsBufferID=0;
GLuint
	IBall::VertsCount = 0; 

void
ProjectMappe::InitIBall(void)
{
	std::vector<glm::vec3> vertsTemp;
	std::vector<glm::vec2> paintTemp;
	std::vector<glm::vec3> normsTemp;
	GLuint temp;
	Utility::loadObj("ball_low_tris.obi",vertsTemp,paintTemp,normsTemp,temp);
	IBall::VertsCount = vertsTemp.size();

	glGenBuffers(1, &IBall::vertsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, IBall::vertsBufferID);
	glBufferData(GL_ARRAY_BUFFER, IBall::VertsCount * sizeof(glm::vec3), &vertsTemp[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IBall::paintBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, IBall::paintBufferID);
	glBufferData(GL_ARRAY_BUFFER, paintTemp.size() * sizeof(glm::vec2), &paintTemp[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IBall::normsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, IBall::normsBufferID);
	glBufferData(GL_ARRAY_BUFFER, normsTemp.size() * sizeof(glm::vec3), &normsTemp[0], GL_STATIC_DRAW);

	_primitivs[PrimitiveType::PRIMITIVE::BALL]->buffers.shape		  = IBall::shape;
	_primitivs[PrimitiveType::PRIMITIVE::BALL]->buffers.VertsCount    = IBall::VertsCount;
	_primitivs[PrimitiveType::PRIMITIVE::BALL]->buffers.vertsBufferID = IBall::vertsBufferID;
	_primitivs[PrimitiveType::PRIMITIVE::BALL]->buffers.paintBufferID = IBall::paintBufferID;
	_primitivs[PrimitiveType::PRIMITIVE::BALL]->buffers.normsBufferID = IBall::normsBufferID;

	_ball.VertsCount    = IBall::VertsCount;
	_ball.vertsBufferID = IBall::vertsBufferID;
	_ball.paintBufferID = IBall::paintBufferID;
	_ball.normsBufferID = IBall::normsBufferID;
}



//////////////////////////////////////////////////////////////////////////////////////
// ZYLINDER:
//////////////////////////////////////////////////////////////////////////////////////

const GLuint
	IZylinder::shape = GL_QUADS;
GLuint
	IZylinder::vertsBufferID = 0;
GLuint
	IZylinder::paintBufferID = 0;
GLuint
	IZylinder::normsBufferID = 0;
GLuint
	IZylinder::VertsCount = 0; 

void
ProjectMappe::InitZylinder(void)
{
	std::vector<glm::vec3> vertsTemp;
	std::vector<glm::vec2> paintTemp;
	std::vector<glm::vec3> normsTemp;

	GLuint temp;
	Utility::loadObj("cylinder_quads.obi",vertsTemp,paintTemp,normsTemp,temp);
	IZylinder::VertsCount = vertsTemp.size();

	glGenBuffers(1, &IZylinder::vertsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, IZylinder::vertsBufferID);
	glBufferData(GL_ARRAY_BUFFER, IZylinder::VertsCount * sizeof(glm::vec3), &vertsTemp[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IZylinder::paintBufferID);
	glBindBuffer(GL_ARRAY_BUFFER,IZylinder::paintBufferID);
	glBufferData(GL_ARRAY_BUFFER, paintTemp.size() * sizeof(glm::vec2), &paintTemp[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IZylinder::normsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, IZylinder::normsBufferID);
	glBufferData(GL_ARRAY_BUFFER, normsTemp.size() * sizeof(glm::vec3), &normsTemp[0], GL_STATIC_DRAW);

	_primitivs[PrimitiveType::PRIMITIVE::ZYLINDER]->buffers.shape		  = IZylinder::shape;
	_primitivs[PrimitiveType::PRIMITIVE::ZYLINDER]->buffers.VertsCount    = IZylinder::VertsCount;
	_primitivs[PrimitiveType::PRIMITIVE::ZYLINDER]->buffers.vertsBufferID = IZylinder::vertsBufferID;
	_primitivs[PrimitiveType::PRIMITIVE::ZYLINDER]->buffers.paintBufferID = IZylinder::paintBufferID;
	_primitivs[PrimitiveType::PRIMITIVE::ZYLINDER]->buffers.normsBufferID = IZylinder::normsBufferID;

	_zylinder.VertsCount    = IZylinder::VertsCount;
	_zylinder.vertsBufferID = IZylinder::vertsBufferID;
	_zylinder.paintBufferID = IZylinder::paintBufferID;
	_zylinder.normsBufferID = IZylinder::normsBufferID;
}




/////////////////////////////////////////////////////////////////////////////////////////
// SPHERE:
/////////////////////////////////////////////////////////////////////////////////////////

const GLuint
	ISphere::shape = GL_QUADS;
GLuint
	ISphere::vertsBufferID=0;
GLuint
	ISphere::paintBufferID=0;
GLuint
	ISphere::normsBufferID=0;
GLuint
	ISphere::VertsCount = 0; 

void
ProjectMappe::InitIspheree(void)
{
	std::vector<glm::vec3> vertsTemp;
	std::vector<glm::vec2> paintTemp;
	std::vector<glm::vec3> normsTemp;

	GLuint temp;
	Utility::loadObj("sphere_quads_low.obi",vertsTemp,paintTemp,normsTemp,temp);
	ISphere::VertsCount = vertsTemp.size();

	glGenBuffers(1, &ISphere::vertsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, ISphere::vertsBufferID);
	glBufferData(GL_ARRAY_BUFFER, ISphere::VertsCount * sizeof(glm::vec3), &vertsTemp[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ISphere::paintBufferID);
	glBindBuffer(GL_ARRAY_BUFFER,ISphere::paintBufferID);
	glBufferData(GL_ARRAY_BUFFER, paintTemp.size() * sizeof(glm::vec2), &paintTemp[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ISphere::normsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, ISphere::normsBufferID);
	glBufferData(GL_ARRAY_BUFFER, normsTemp.size() * sizeof(glm::vec3), &normsTemp[0], GL_STATIC_DRAW);

	_primitivs[PrimitiveType::PRIMITIVE::SPHERE]->buffers.shape		    = ISphere::shape;
	_primitivs[PrimitiveType::PRIMITIVE::SPHERE]->buffers.VertsCount    = ISphere::VertsCount;
	_primitivs[PrimitiveType::PRIMITIVE::SPHERE]->buffers.vertsBufferID = ISphere::vertsBufferID;
	_primitivs[PrimitiveType::PRIMITIVE::SPHERE]->buffers.paintBufferID = ISphere::paintBufferID;
	_primitivs[PrimitiveType::PRIMITIVE::SPHERE]->buffers.normsBufferID = ISphere::normsBufferID;

	_sphere.VertsCount    = ISphere::VertsCount;
	_sphere.vertsBufferID = ISphere::vertsBufferID;
	_sphere.paintBufferID = ISphere::paintBufferID;
	_sphere.normsBufferID = ISphere::normsBufferID;
}




 ////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CONE:
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

const GLuint 
	ICone::shape = GL_TRIANGLES;
GLuint 
	ICone::vertsBufferID = 0;
GLuint 
	ICone::paintBufferID = 0;
GLuint 
	ICone::normsBufferID = 0;
GLuint 
	ICone::VertsCount = 0;

void
ProjectMappe::InitICone(void)
{
	std::vector<glm::vec3> vertsTemp;
	std::vector<glm::vec2> paintTemp;
	std::vector<glm::vec3> normsTemp;

	GLuint temp;
	Utility::loadObj("cone_tris_32.obi",vertsTemp,paintTemp,normsTemp,temp);
	ICone::VertsCount = vertsTemp.size();

	glGenBuffers(1, &ICone::vertsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, ICone::vertsBufferID);
	glBufferData(GL_ARRAY_BUFFER, ICone::VertsCount * sizeof(glm::vec3), &vertsTemp[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ICone::paintBufferID);
	glBindBuffer(GL_ARRAY_BUFFER,ICone::paintBufferID);
	glBufferData(GL_ARRAY_BUFFER, paintTemp.size() * sizeof(glm::vec2), &paintTemp[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ICone::normsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, ICone::normsBufferID);
	glBufferData(GL_ARRAY_BUFFER, normsTemp.size() * sizeof(glm::vec3), &normsTemp[0], GL_STATIC_DRAW);

	_primitivs[PrimitiveType::PRIMITIVE::CONE]->buffers.shape		  = ICone::shape;
	_primitivs[PrimitiveType::PRIMITIVE::CONE]->buffers.VertsCount    = ICone::VertsCount;
	_primitivs[PrimitiveType::PRIMITIVE::CONE]->buffers.vertsBufferID = ICone::vertsBufferID;
	_primitivs[PrimitiveType::PRIMITIVE::CONE]->buffers.paintBufferID = ICone::paintBufferID;
	_primitivs[PrimitiveType::PRIMITIVE::CONE]->buffers.normsBufferID = ICone::normsBufferID;

	_cone.VertsCount    = ICone::VertsCount;
	_cone.vertsBufferID = ICone::vertsBufferID;
	_cone.paintBufferID = ICone::paintBufferID;
	_cone.normsBufferID = ICone::normsBufferID;
}							



//////////////////////////////////////////////////////////////////////////////////////////////////////////
// CAPSULE:
//////////////////////////////////////////////////////////////////////////////////////////////////////////

const GLuint 
	ICapsule::shape = GL_QUADS;
GLuint 
	ICapsule::vertsBufferID = 0;
GLuint 
	ICapsule::paintBufferID = 0;
GLuint 
	ICapsule::normsBufferID = 0;
GLuint 
	ICapsule::VertsCount = 0;

void
ProjectMappe::InitICapsule(void)
{
	std::vector<glm::vec3> vertsTemp;
	std::vector<glm::vec2> paintTemp;
	std::vector<glm::vec3> normsTemp;

	GLuint temp;
	Utility::loadObj("capsula_quads_32.obi",vertsTemp,paintTemp,normsTemp,temp);
	ICapsule::VertsCount = vertsTemp.size();

	glGenBuffers(1, &ICapsule::vertsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, ICapsule::vertsBufferID);
	glBufferData(GL_ARRAY_BUFFER, ICapsule::VertsCount * sizeof(glm::vec3), &vertsTemp[0], GL_STATIC_DRAW);
	
	glGenBuffers(1, &ICapsule::paintBufferID);
	glBindBuffer(GL_ARRAY_BUFFER,ICapsule::paintBufferID);
	glBufferData(GL_ARRAY_BUFFER, paintTemp.size() * sizeof(glm::vec2), &paintTemp[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ICapsule::normsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, ICapsule::normsBufferID);
	glBufferData(GL_ARRAY_BUFFER, normsTemp.size() * sizeof(glm::vec3), &normsTemp[0], GL_STATIC_DRAW);

	_primitivs[PrimitiveType::PRIMITIVE::CAPSULA]->buffers.shape	     = ICapsule::shape;
	_primitivs[PrimitiveType::PRIMITIVE::CAPSULA]->buffers.VertsCount    = ICapsule::VertsCount;
	_primitivs[PrimitiveType::PRIMITIVE::CAPSULA]->buffers.vertsBufferID = ICapsule::vertsBufferID;
	_primitivs[PrimitiveType::PRIMITIVE::CAPSULA]->buffers.paintBufferID = ICapsule::paintBufferID;
	_primitivs[PrimitiveType::PRIMITIVE::CAPSULA]->buffers.normsBufferID = ICapsule::normsBufferID;

	_capsula.VertsCount    = ICapsule::VertsCount;
	_capsula.vertsBufferID = ICapsule::vertsBufferID;
	_capsula.paintBufferID = ICapsule::paintBufferID;
	_capsula.normsBufferID = ICapsule::normsBufferID;
}




//////////////////////////////////////////////////////////////
// Pyramid static definitions:
//////////////////////////////////////////////////////////////


const GLuint
	IPyramidal::shape = GL_TRIANGLE_STRIP;
const GLuint 
	IPyramidal::VertsCount = 6;
GLuint
	IPyramidal::vertsBufferID = 0; 
GLuint
	IPyramidal::paintBufferID = 0;
GLuint
	IPyramidal::normsBufferID = 0;



// Initiation-function for IFlatquad-static's
void
ProjectMappe::InitiPyramidi(void)
{
	dreier vertsTemp[IPyramidal::VertsCount];
	zweier paintTemp[IPyramidal::VertsCount];
	dreier normsTemp[4];

		
	vertsTemp[0][0] = 0;
	vertsTemp[0][1] = -0.5f;
	vertsTemp[0][2] = -1.2f;
	paintTemp[0][0] = 0;
	paintTemp[0][1] = 0;

	vertsTemp[1][0] = 1.039230f;
	vertsTemp[1][1] = -0.5f;
	vertsTemp[1][2] = 0.6f;
	paintTemp[1][0] = 1;
	paintTemp[1][1] = 0;

	vertsTemp[2][0] = 0;  
	vertsTemp[2][1] = 1.053666f;
	vertsTemp[2][2] = 0;
	paintTemp[2][0] = 1;
	paintTemp[2][1] = 1;

	vertsTemp[3][0] = -1.039230f;
	vertsTemp[3][1] = -0.5f;
	vertsTemp[3][2] = 0.6f;
	paintTemp[3][0] = 0;
	paintTemp[3][1] = 1;

	vertsTemp[4][0] = 0;
	vertsTemp[4][1] = -0.5f;
	vertsTemp[4][2] = -1.2f;
	paintTemp[4][0] = 0;
	paintTemp[4][1] = 0;

	vertsTemp[5][0] = 1.039230f;
	vertsTemp[5][1] = -0.5f;
	vertsTemp[5][2] = 0.6f;
	paintTemp[5][0] = 1;
	paintTemp[5][1] = 0;


	normsTemp[0][0] = -0;
	normsTemp[0][1] = 0.360300f;
	normsTemp[0][2] = 0.932900f;
	normsTemp[1][0] = 0.807900f; 
	normsTemp[1][1] = 0.360300f; 
	normsTemp[1][2] = -0.466400f;
	normsTemp[2][0] = -0.807900f;
	normsTemp[2][1] = 0.360300f;
	normsTemp[2][2] = -0.466400f;
	normsTemp[3][0] = 0;
	normsTemp[3][1] = -1;
	normsTemp[3][2] = 0;

	glGenBuffers(1, &IPyramidal::vertsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, IPyramidal::vertsBufferID);
	glBufferData(GL_ARRAY_BUFFER, IPyramidal::VertsCount * sizeof(dreier), &vertsTemp[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IPyramidal::paintBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, IPyramidal::paintBufferID);
	glBufferData(GL_ARRAY_BUFFER, IPyramidal::VertsCount * sizeof(zweier), &paintTemp[0], GL_STATIC_DRAW);

	glGenBuffers(1, &IPyramidal::normsBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, IPyramidal::normsBufferID);
	glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(dreier), &normsTemp[0], GL_STATIC_DRAW);

	_primitivs[PrimitiveType::PRIMITIVE::PYRAMID]->buffers.vertsBufferID = IPyramidal::vertsBufferID;
	_primitivs[PrimitiveType::PRIMITIVE::PYRAMID]->buffers.paintBufferID = IPyramidal::paintBufferID;
	_primitivs[PrimitiveType::PRIMITIVE::PYRAMID]->buffers.normsBufferID = IPyramidal::normsBufferID;
	_primitivs[PrimitiveType::PRIMITIVE::PYRAMID]->buffers.VertsCount	 = IPyramidal::VertsCount;
	_primitivs[PrimitiveType::PRIMITIVE::PYRAMID]->buffers.shape	     = IPyramidal::shape;

	_flatquad.vertsBufferID = IPyramidal::vertsBufferID;
	_flatquad.paintBufferID = IPyramidal::paintBufferID;
	_flatquad.normsBufferID = IPyramidal::normsBufferID;
}

//EOF



	