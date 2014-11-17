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
	GLboolean NoBackfaceCulling;
	GLuint FaceShape;
	
	GLuint vertexBufferID;
	GLuint uvBufferID;
	Texture texture;
	std::vector<glm::vec3> verts;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> norms;

public:
	GLboolean UseTexture;
	IMeshObject(void);
	virtual ~IMeshObject(void);
	virtual void draw(void);
	void InitializeObject(const char* objFile,bool addToSceneGraph=true);
	virtual void LoadMesh(const char* objFileName);
	virtual IGObject* LoadTexture(const char* textureFileName);
	virtual void SetColor(data32 color);
	virtual void SetColor(unsigned char,unsigned char,unsigned char,unsigned char);
	virtual Vector3 move(Vector3);
	virtual Vector3 move(float,float,float);
	virtual Vector3 scale(Vector3);
	virtual Vector3 rotate(Vector3);
	virtual Vector3 rotate(float,float,float);
	data32 color;

	
};

#endif
