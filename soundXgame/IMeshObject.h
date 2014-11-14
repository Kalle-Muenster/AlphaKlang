#ifndef _MESHOBJECT_H_
#define _MESHOBJECT_H_

#include "igobject.h"


class IMeshObject : 
	public IGObject
{


protected:
	GLboolean UseTransparenz;
	GLuint FaceShape;
	GLuint vertexBufferID;
	GLuint uvBufferID;
	GLuint textureID;
	Color color;
	std::vector<glm::vec3> verts;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> norms;

public:

	IMeshObject(void);
	virtual ~IMeshObject(void);
	virtual void draw(void);
	void InitializeObject(const char* objFile,bool addToSceneGraph=true);
	virtual void LoadMesh(const char* objFileName);
	virtual IGObject* LoadTexture(const char* textureFileName);
	virtual void SetColor(Color);
	virtual Vector3 move(Vector3);
	virtual Vector3 move(float,float,float);
	virtual Vector3 scale(Vector3);
	virtual Vector3 rotate(Vector3);
	virtual Vector3 rotate(float,float,float);

	
};

#endif
