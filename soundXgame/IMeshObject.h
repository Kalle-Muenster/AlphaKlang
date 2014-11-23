#ifndef _MESHOBJECT_H_
#define _MESHOBJECT_H_

#include "igobject.h"

struct Texture 
{
	enum class Format
	{DUAL=1,GRAY=16,RGB=24,RGBA=32};
	GLuint ID;
	short w,h;
	Format format;

	void Load(string fileName,short Width,short Height, Format textureFormat=Format::RGBA);
};

class IMeshObject : 
	public IGObject
{

protected:

	GLuint					FaceShape;
	GLuint					vertexBufferID;
	GLuint					uvBufferID;
	GLboolean				NoBackfaceCulling;
	GLboolean				UseTexture;
	Texture					texture;
	std::vector<glm::vec3>	verts;
	std::vector<glm::vec2>	uvs;
	std::vector<glm::vec3>	norms;

public:
			IMeshObject(void);
	void	InitializeObject(const char* objFile,bool addToSceneGraph=true,Vector3 positionOffset = *Vector3::Zero);
	virtual ~IMeshObject(void);


	data32				color;

	virtual void		LoadMesh(const char* objFileName);
	virtual void		LoadMesh(const char* objFileName,Vector3 positionOffset);
	virtual IGObject*	LoadTexture(const char* textureFileName);
	virtual void		SetColor(data32 color);
	virtual void		SetColor(unsigned char,unsigned char,unsigned char,unsigned char);
	virtual Vector3		move(Vector3);
	virtual Vector3		move(float,float,float);
	virtual Vector3		rotate(Vector3);
	virtual Vector3		rotate(float,float,float);
	virtual Vector3		scale(Vector3);
	virtual Vector3		scale(float,float,float);
	virtual void		draw(void);

};
//class Kollective : 
//	public IConnectable
//{
//private:
//	
//	unsigned portID;
//	bool progressing;
//	IGObject* instance;
//public:
//	unsigned count;
//	bool checkFail(void)
//	{return progressing;}
//	bool through(GobID wire)
//	{return wire != portID;}
//	void apear(IGObject* numberOfCount)
//	{ instance = numberOfCount; progressing = false; }
//	IGObject* get(unsigned number,unsigned ofCount)
//	{
//		portID = number;
//		count = ofCount;
//		progressing = true;
//
//		while(progressing)
//			this->Connection()->through();
//
//		return instance;
//	}
//};
#endif
