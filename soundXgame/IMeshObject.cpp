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
	this->color.u32 = 0xffffffff;
	this->FaceShape = GL_TRIANGLES;
	this->NoBackfaceCulling = false;
	this->UseTexture = false;
}

IMeshObject::~IMeshObject(void)
{

}

void
IMeshObject::LoadMesh(const char* objFileName)
{
	
	this->SetName(Utility::loadObj(objFileName,this->verts,this->uvs,this->norms,this->FaceShape));



	
	transform.right	  = glm::vec3(1,0,0);
	transform.up	  = glm::vec3(0,1,0);
	transform.forward = glm::vec3(0,0,1);

	//verts.push_back(transform.right);
	//verts.push_back(transform.up);
	//verts.push_back(transform.forward);



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
IMeshObject::SetColor(unsigned char r,unsigned char g,unsigned char b,unsigned char a)
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
IMeshObject::InitializeObject(const char* objFile,bool addToSceneGraph)
{
	if(addToSceneGraph)
		SetID(SCENE->Add(this));
	LoadMesh(objFile);
	LockID();
}


Vector3
IMeshObject::move(Vector3 to)
{
	return move(to.x,to.y,to.z);
}

Vector3
IMeshObject::move(float tox,float toy,float toz)
{

	getTransform()->movement.x = (tox - getTransform()->position.x);
	getTransform()->movement.y = (toy - getTransform()->position.y);
	getTransform()->movement.z = (toz - getTransform()->position.z);

	if(getTransform()->movement.x!=0||getTransform()->movement.y!=0||getTransform()->movement.x!=0)
	{
		getTransform()->position.x = tox;
		getTransform()->position.y = toy;
		getTransform()->position.z = toz;

#ifdef DEBUG_OUTPUT_MESHOBJECTS
	printf("\n%s - ID: %i Has Moved to %f,%f,%f !",GetName(),GetID(),getTransform()->position.x,getTransform()->position.y,getTransform()->position.z);
#endif

	
	}

	return getTransform()->position;
}


Vector3
IMeshObject::scale(Vector3 to)
{
	getTransform()->scale = to;
	return getTransform()->scale;
}

Vector3
IMeshObject::rotate(Vector3 to)
{
	getTransform()->rotation = to;
	return getTransform()->rotation;
}

Vector3
IMeshObject::rotate(float toiX,float toYps,float toZed)
{ 
	rotate(Vector3(toiX,toYps,toZed));
	//getTransform()->rotation.x = toiX;
	//getTransform()->rotation.y = toYps;
	//getTransform()->rotation.z = toZed;
	//getTransform()->forward = glm::normalize((glm::vec3)getTransform()->rotation);
	//
	return getTransform()->rotation;
}

void
IMeshObject::draw(void)
{


	if(!IsVisible)
		return;
	
	if(!vertexBufferID)
		return;

	if(NoBackfaceCulling)
		glDisable(GL_CULL_FACE);
	else
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	//glEnable(GL_TEXTURE_2D);
	if(UseTexture)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture.ID);
	}
	else
	{
	//	glColor4f(0.1f,0.2f,0.3f,1.0f);
		glColor4b(color.byte[1],color.byte[2],color.byte[3],color.byte[0]);
	}
//	else
	//	glDisable(GL_TEXTURE_2D);





	glPushMatrix();
	{
//	if(UseTexture)
//	{
		

		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
		glVertexPointer(3, GL_FLOAT, 0, 0);
	
		
		glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
		glTexCoordPointer(2, GL_FLOAT, 0, 0);
//	}
//	else
//	{
	//	
//		glColor4f(*this->color.R/255,*this->color.G/255,*this->color.B/255,*this->color.A/255);
//	}
		
		//Translate...
		glTranslatef(this->getTransform()->position.x, this->getTransform()->position.y,this->getTransform()->position.z);

		if(this->IsGrounded)
		{
			float y = Ground::getInstance()->GetGroundY(this->transform.position.x, this->transform.position.z);
			glTranslatef(0, y, 0);
		}

		//Rotate...
		glRotatef(this->getTransform()->rotation.x, 1, 0, 0);
		glRotatef(this->getTransform()->rotation.y, 0, 1, 0);
		glRotatef(this->getTransform()->rotation.z, 0, 0, 1);

		
		//Scaleate...
		glScalef(this->getTransform()->scale.x,this->getTransform()->scale.y,this->getTransform()->scale.z);
		
		//Draw
		glDrawArrays(this->FaceShape, 0, verts.size());
	}
	glPopMatrix();

    

	if(UseTexture)
		glDisable(GL_TEXTURE_2D);
	else
		glColor4f(1,1,1,1);
}

//Texture Struct:
//############################################
void Texture::Load(string fileName,short Width,short Height,Format textureFormat)
{
	ID = Utility::loadTexture(fileName);
	w=Width;
	h=Height;
	format=textureFormat;
}