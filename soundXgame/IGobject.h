#ifndef __ISCHMENA_H__
#define __ISCHMENA_H__

#include <vector>
#include "projectGrafics.h"
#include "DataStructs.h"

class IConnectable;
struct Transform;
struct Vector3;

typedef unsigned int GobID;

class IGObject
{
private:
	GobID ID;
	bool _idIsSet;

protected:
	Transform transform;
	char Name[64];
	bool SetID(GobID);
	void LockID(void);

public:
	IGObject(void);
	virtual ~IGObject(void);
	void InitializeObject(bool addToSceneGraph=true);
	bool IsVisible;
	virtual void draw(void)=0;
	Transform* getTransform(void);
	virtual Vector3 move(float,float,float) {return Vector3();};//=0;
	virtual Vector3 rotate(float,float,float) {return Vector3();};//=0;
	virtual Vector3 scale(Vector3) {return Vector3();};//=0;
	GobID GetID(void);
	char* GetName(void);
	void SetName(char*);
	IConnectable* conXtor;
	operator IConnectable();
	bool IsGrounded;

	template<typename T> T* AddConnectable(void)
	{
		return conXtor->AddConnectable<T>();
	}
	template<typename T> T* GetConnected(void)
	{
		return conXtor->GetConnected<T>();
	}
	template<typename T> bool HasConnected(void)
	{
		return conXtor->GetConnected<T>()!=NULL;
	}
};



class IMeshObject : 
	public IGObject
{


protected:
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
	virtual void draw();
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










