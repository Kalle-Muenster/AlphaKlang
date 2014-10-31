#ifndef __ISCHMENA_H__
#define __ISCHMENA_H__


#include <vector>
#include <iostream>

#include "projectGrafics.h"
#include "Utility.h"
#include "UpdateManager.h"

class IConnectable;


typedef unsigned GobID;

class AbsGobject
{
protected:
	TransformA transform;
	bool _idIsSet;
	GobID ID;
	char Name[64];

public:
	AbsGobject(void);
	virtual ~AbsGobject(void)=0{}
	bool IsVisible;
	virtual void draw(void)=0;
	TransformA* getTransform(void);
	virtual Vector3 move(float,float,float)=0;
	virtual Vector3 rotate(float,float,float)=0;
	virtual Vector3 scale(Vector3)=0;
	GobID GetObjectID(void);
	char* GetName(void);
	void SetName(char*);
	
};

class IGobject : 
	public AbsGobject
{
protected:

	GLuint vertexBufferID;
	GLuint uvBufferID;
	GLuint textureID;
	
	std::vector<glm::vec3> verts;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> norms;

public:
//	TransformA transform;
	//virtual TransformA* getTransform(void);
	//bool IsVisible;
	IGobject(void);
	virtual ~IGobject(void);
	virtual void draw();
	virtual void init(const char* objFile,const char* textureFile);
	virtual void init(const char* objFile,const char* textureFile,bool addToSceneGraph);
	virtual Vector3 move(Vector3);
	virtual Vector3 move(float,float,float);
	virtual Vector3 scale(Vector3);
	virtual Vector3 rotate(Vector3);
	virtual Vector3 rotate(float,float,float);
	IConnectable* conXtor;
	

};



#endif
