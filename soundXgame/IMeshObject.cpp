#include <iostream>

#include "projectMacros.h"
#include "DataStructs.h"
#include "Utility.h"
#include "Connectable.h"
#include "IGObject.h"
#include "IMeshObject.h"
#include "Ground.h"



ICubeXtor::ICubeXtor(void)
{
	TypeHashCode = (unsigned) typeid(ICubeXtor).hash_code();
	ConnectionID = 0;
	UseTexture=false;
	NoBackfaceCulling=false;
}

bool
ICubeXtor::useTexture(BOOL use)
{
	if(use<3)
		UseTexture=use;
	return UseTexture;
}

void
ICubeXtor::draw(void)
{		

}

IObjection<IConnectable>*  
ICubeXtor::Connection(void)
{
	return (IObjection<IConnectable>*) this->connection;
}



IMeshObject::IMeshObject(void)
{
	angle = 0.0f;
	this->transform.scale = Vector3(1,1,1);
	transform.right	  = Vector3(1,0,0);
	transform.up	  = Vector3(0,1,0);
	transform.forward = Vector3(0,0,-1);
	transform.speed = 1.f;
	this->color.u32 = 0xffffffff;
	this->FaceShape = GL_TRIANGLES;
	this->NoBackfaceCulling = false;
	this->UseTexture = false;
	this->GroundedWithPivot = false;
	this->Pivot = *Vector3::Zero;
}

IMeshObject::~IMeshObject(void)
{

}

void
IMeshObject::LoadMesh(const char* objFileName)
{
	this->LoadObj(objFileName);
	this->DoObjBuffer();
}

void
IMeshObject::LoadMesh(const char* objFileName,Vector3 positionOffset)
{
	this->LoadObj(objFileName);
	this->Pivot = positionOffset;
	int count = verts.size();
	for(int i = 0;i < count;i++)
	{
		verts.at(i) += (glm::vec3)positionOffset;
		//verts.at(i).x += positionOffset.x;
		//verts.at(i).y += positionOffset.y;
		//verts.at(i).z += positionOffset.z;
	}
	this->DoObjBuffer();
}

void
IMeshObject::LoadObj(const char* objFileName)
{
	this->SetName(Utility::loadObj(objFileName,this->verts,this->uvs,this->norms,this->FaceShape));
}

void
IMeshObject::DoObjBuffer(void)
{
	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec3), &verts[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
}

void
IMeshObject::SetColor(data32 newColor)
{
	color = newColor;
}

void
IMeshObject::SetColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	color.byte[0] = a;
	color.byte[1] = r;
	color.byte[2] = g;
	color.byte[3] = b;
}

IGObject*
IMeshObject::LoadTexture(const char* textureFile)
{
	int i = -1;
	while((++i<64)&&(textureFile[i]!='_'));
	if(i<64)
		sscanf(&textureFile[i],"_%ix%i.",&texture.w,&texture.h);

	this->texture.ID = Utility::loadTexture(textureFile);
	UseTexture = true;
	return this;
}

void
IMeshObject::InitializeObject(const char* objFile, bool addToSceneGraph, Vector3 positionOffset)
{
	if(addToSceneGraph)
	{
		SetID(SCENE->Add(this));
		LockID();
	}
	LoadMesh(objFile,positionOffset);
	
}

Texture*
IMeshObject::GetTexture(void)
{
	return &texture;
}

void 
IMeshObject::SetTexture(Texture* tex)
{
	 texture = Texture(*tex);

}

Vector3
IMeshObject::move(Vector3 to)
{
	//getTransform()->movement = (to - getTransform()->position);
	//getTransform()->position = to;
	//return getTransform()->position;
	return IGObject::move(to);
}

Vector3
IMeshObject::move(float tox, float toy, float toz)
{
	return move(Vector3(tox,toy,toz));
}

Vector3
IMeshObject::scale(Vector3 to)
{
	getTransform()->scale = to;
	return getTransform()->scale;
}

Vector3
IMeshObject::scale(float X,float Y,float Z)
{
	return scale(Vector3(X,Y,Z));
}

Vector3
IMeshObject::rotate(Vector3 to)
{
	getTransform()->rotation = to;
	if(!AlwaysFaceMovingdirection)
		getTransform()->forward = (to - getTransform()->position).normalized();
	return getTransform()->rotation;
}

Vector3
IMeshObject::rotate(float toiX,float toYps,float toZed)
{ 
	return rotate(Vector3(toiX,toYps,toZed));
}



void
IMeshObject::draw(void)
{
	if(!vertexBufferID)
		return;

	if(NoBackfaceCulling == false)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	if(UseTexture)
	{
		glColor3f(1,1,1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture.ID);
	}
	else
	{
		glColor4b(color.byte[1],color.byte[2],color.byte[3],color.byte[0]);
	}

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	
	glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);

	glPushMatrix();
	{
		
		Vector3 values = getTransform()->position;

		//Grounding:
		if(IsGrounded())
		{
			GLfloat *y = &getTransform()->position.y;
			*y -= this->GroundValue;
			this->GroundValue = GroundedWithPivot ? 
								Ground::getInstance()->GetGroundY(values.x + Pivot.x * getTransform()->scale.x, values.z + Pivot.z * getTransform()->scale.z) 
								:
								Ground::getInstance()->GetGroundY(values.x, values.z);

			*y += this->GroundValue;
		}


		// Translation:
		glTranslatef(values.x, values.y, values.z);

		// Rotation:
		values = getTransform()->rotation;
		if(angle==0) //by the transform's rotation-values as angles at their coresponding global-axis...
		{
			glRotatef(values.x, 1, 0, 0);
			glRotatef(values.y, 0, 1, 0);
			glRotatef(values.z, 0, 0, -1);
		}
		else // if angle is not zero, rotation will be aplied as rotation around an axis wich's defined by the transform's rotation-values.
			glRotatef(angle,values.x,values.y,values.z);
		
		// Scaling:
		values = getTransform()->scale;
		glScalef(values.x,values.y,values.z);
		
		// Draw
		glDrawArrays(FaceShape, 0, verts.size());
	}
	glPopMatrix();

	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
}



