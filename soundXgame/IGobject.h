#ifndef __ISCHMENA_H__
#define __ISCHMENA_H__

#include <vector>
#include "projectGrafics.h"
#include "DataStructs.h"

class IConnectable;
struct Transform;
struct Vector3;

typedef unsigned int GobID;

class IGobject
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
	IGobject(void);
	virtual ~IGobject(void);
	bool IsVisible;
	virtual void draw(void)=0;
	Transform* getTransform(void);
	virtual Vector3 move(float,float,float)=0;
	virtual Vector3 rotate(float,float,float)=0;
	virtual Vector3 scale(Vector3)=0;
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



class IMeshGobject : 
	public IGobject
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

	IMeshGobject(void);
	virtual ~IMeshGobject(void);
	virtual void draw();
	virtual void InitializeObject(const char* objFile,bool addToSceneGraph=true);
	virtual void LoadMesh(const char* objFileName);
	virtual IGobject* LoadTexture(const char* textureFileName);
	virtual void SetColor(Color);
	virtual Vector3 move(Vector3);
	virtual Vector3 move(float,float,float);
	virtual Vector3 scale(Vector3);
	virtual Vector3 rotate(Vector3);
	virtual Vector3 rotate(float,float,float);

	
};



#endif










//
//
//#ifndef __ISCHMENA_H__
//#define __ISCHMENA_H__
//
//#include <vector>
//#include "projectGrafics.h"
//#include "DataStructs.h"
//
//class IConnectable;
//struct Transform;
//struct Vector3;
//
//typedef unsigned int GobID;
//
//class IGobject
//{
//private:
//	GobID ID;
//	bool _idIsSet;
//
//protected:
//	Transform transform;
//	char Name[64];
//	bool SetID(GobID);
//	void LockID(void);
//
//public:
//	IGobject(void);
//	virtual ~IGobject(void);
//	bool IsVisible;
//	virtual void draw(void)=0;
//	Transform* getTransform(void);
//	virtual Vector3 move(float,float,float)=0;
//	virtual Vector3 rotate(float,float,float)=0;
//	virtual Vector3 scale(Vector3)=0;
//	GobID GetID(void);
//	char* GetName(void);
//	void SetName(char*);
//	IConnectable* conXtor;
//	operator IConnectable();
//
//	template<typename T> T* AddConnectable(void)
//	{
//		return conXtor->AddConnectable<T>();
//	}
//	template<typename T> T* GetConnected(void)
//	{
//		return conXtor->GetConnected<T>();
//	}
//	template<typename T> bool HasConnected(void)
//	{
//		return conXtor->GetConnected<T>()!=NULL;
//	}
//};
//
//class IMeshGobject : 
//	public IGobject
//{
//protected:
//
//	GLuint vertexBufferID;
//	GLuint uvBufferID;
//	GLuint textureID;
//	
//	std::vector<glm::vec3> verts;
//	std::vector<glm::vec2> uvs;
//	std::vector<glm::vec3> norms;
//
//public:
//
//	IMeshGobject(void);
//	virtual ~IMeshGobject(void);
//	virtual void draw();
//	virtual void init(const char* objFile,const char* textureFile);
//	virtual void init(const char* objFile,const char* textureFile,bool addToSceneGraph);
//	virtual Vector3 move(Vector3);
//	virtual Vector3 move(float,float,float);
//	virtual Vector3 scale(Vector3);
//	virtual Vector3 rotate(Vector3);
//	virtual Vector3 rotate(float,float,float);
//};
//
//
//
//#endif
