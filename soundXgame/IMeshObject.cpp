#include <iostream>

#include "projectMacros.h"
#include "DataStructs.h"
#include "Utility.h"
#include "Connectable.h"
#include "IGObject.h"
#include "IMeshObject.h"
#include "Ground.h"

IMeshObject::IMeshObject(void)
{
	this->transform.scale = Vector3(1,1,1);
	transform.right	  = glm::vec3(1,0,0);
	transform.up	  = glm::vec3(0,1,0);
	transform.forward = glm::vec3(0,0,1);
	transform.speed=1.f;
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
		verts.at(i).x += positionOffset.x;
		verts.at(i).y += positionOffset.y;
		verts.at(i).z += positionOffset.z;
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
	this->color = newColor;
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
	this->texture.ID = Utility::loadTexture(textureFile);
	UseTexture = true;
	return this;
}

void
IMeshObject::InitializeObject(const char* objFile, bool addToSceneGraph, Vector3 positionOffset)
{
	if(addToSceneGraph)
		SetID(SCENE->Add(this));
	LoadMesh(objFile,positionOffset);
	LockID();
}

Vector3
IMeshObject::move(Vector3 to)
{
	return move(to.x,to.y,to.z);
}

Vector3
IMeshObject::move(float tox, float toy, float toz)
{
	getTransform()->movement.x = (tox - getTransform()->position.x);
	getTransform()->movement.y = (toy - getTransform()->position.y);
	getTransform()->movement.z = (toz - getTransform()->position.z);

	if(getTransform()->movement.x!=0||getTransform()->movement.y!=0||getTransform()->movement.z!=0)
	{
		getTransform()->position.x = tox;
		getTransform()->position.y = toy;
		getTransform()->position.z = toz;

		#ifdef DEBUG_OUTPUT_MESHOBJECTS
			printf("\n%s - ID: %i Has Moved to %f,%f,%f !",GetName(),GetID(),getTransform()->position.x,getTransform()->position.y,getTransform()->position.z);
		#endif
	}

	return getTransform()->movement;
}

Vector3
IMeshObject::scale(Vector3 to)
{
	return scale(to.x,to.y,to.z);
}

Vector3
IMeshObject::scale(float X,float Y,float Z)
{
	getTransform()->scale.x=X;
	getTransform()->scale.y=Y;
	getTransform()->scale.z=Z;	
	return getTransform()->scale;
}

Vector3
IMeshObject::rotate(Vector3 to)
{
	rotate(to.x,to.y,to.z);
	return getTransform()->rotation;
}

Vector3
IMeshObject::rotate(float toiX,float toYps,float toZed)
{ 
	getTransform()->rotation.x = toiX;
	getTransform()->rotation.y = toYps;
	getTransform()->rotation.z = toZed;

	getTransform()->forward = (getTransform()->rotation - getTransform()->position);
	return  getTransform()->rotation;
}

void
IMeshObject::draw(void)
{
	//if(!IsVisible) // <- check already within SceneGraph
		//return;
	
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
	
	//if(UseTexture)
	//{
		glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
		glTexCoordPointer(2, GL_FLOAT, 0, 0);
	//}

	glPushMatrix();
	{
		// Translate
		GLfloat x  = getTransform()->position.x;
		GLfloat z = getTransform()->position.z;
		GLfloat y = getTransform()->position.y;

		// Grounded
		if(IsGrounded)
		{
			if(GroundedWithPivot)
			{
				y += Ground::getInstance()->GetGroundY(x + Pivot.x * getTransform()->scale.x, z + Pivot.z * getTransform()->scale.z);
			}
			else
			{
				y += Ground::getInstance()->GetGroundY(x, z);
			}
		}
		
		glTranslatef(x, y, z);

		// Rotate
		glRotatef(getTransform()->rotation.x, 1, 0, 0);
		glRotatef(getTransform()->rotation.y, 0, 1, 0);
		glRotatef(getTransform()->rotation.z, 0, 0, 1);

		// Scale
		glScalef(getTransform()->scale.x,getTransform()->scale.y,getTransform()->scale.z);
		
		// Draw
		glDrawArrays(FaceShape, 0, verts.size());
	}
	glPopMatrix();

	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
}

//Texture Struct:
//############################################
void Texture::Load(string fileName,short Width,short Height,Format textureFormat)
{
	ID = Utility::loadTexture(fileName);
	w = Width;
	h = Height;
	format=textureFormat;
}