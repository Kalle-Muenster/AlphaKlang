#include <iostream>
#include "projectMacros.h"
#include "DataStructs.h"
#include "Utility.h"
#include "Connectable.h"
#include "IGObject.h"

#include "Ground.h"

#define This this->conXtor->Connection()
static unsigned objIDs = 99;
std::vector<GobID> usedIDs = std::vector<GobID>();

bool
_IDIsFree(GobID id)
{
	for(auto it=usedIDs.begin();it!=usedIDs.end();it++)
		if((*it)==id)
			return false;
	return true;
}

IGObject::IGObject(void)
{
	IsVisible=false;
	this->_idIsSet=false;
	conXtor = new IConnectable();
	conXtor->SetConnection(this);
	IsGrounded=true;

}

IGObject::~IGObject(void)
{
	delete conXtor;
}

bool
IGObject::SetID(GobID id)
{
	if(this->_idIsSet==false)
	{
		this->ID = id;
		itoa(this->ID,&this->Name[0],10);
		usedIDs.push_back(id);
		return this->_idIsSet=true;
	}
	else
		return false;
}

void
IGObject::LockID(void)
{
	if(!_idIsSet)
	{
		while(!_IDIsFree(++objIDs));
		this->ID = objIDs;
		usedIDs.push_back(this->ID);
		itoa(this->ID,&this->Name[0],10);
		this->_idIsSet=true;
	}
}

void
IGObject::InitializeObject(bool addToSceneGraph)
{
	if(addToSceneGraph)
		SetID(SCENE->Add(this));

	LockID();
}


IMeshObject::IMeshObject(void)
{
	this->transform.scale = Vector3(1,1,1);
	this->color = Color(128,128,128,255);
	this->FaceShape = GL_TRIANGLES;
}

IMeshObject::~IMeshObject(void)
{

}

void
IMeshObject::LoadMesh(const char* objFileName)
{
	
	this->SetName(Utility::loadObj(objFileName,this->verts,this->uvs,this->norms,this->FaceShape));

	glm::vec3 temp1 = glm::vec3(0,0,-1);
	glm::vec3 temp2 = glm::vec3(1,0,0);
	glm::vec3 temp3 = glm::vec3(0,1,0);

	transform.forward = Vector3(temp1.x,temp1.y,temp1.z);
	transform.right = temp2;
	transform.up = temp3;

	verts.push_back(glm::vec3(temp1.x,temp1.y,temp1.z));
	verts.push_back(glm::vec3(temp2.x,temp2.y,temp2.z));
	verts.push_back(glm::vec3(temp3.x,temp3.y,temp3.z));

	glGenBuffers(1, &vertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(glm::vec3), &verts[0], GL_STATIC_DRAW);

	glGenBuffers(1, &uvBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
}

void
IMeshObject::SetColor(Color newColor)
{
	this->color = newColor;
}

IGObject*
IMeshObject::LoadTexture(const char* textureFile)
{

	this->textureID = Utility::loadTexture(textureFile);
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







GobID
IGObject::GetID(void)
{
	return this->ID;
}



char* 
IGObject::GetName(void)
{
	return &this->Name[0];
}

void
IGObject::SetName(char* name)
{
	int i = 0;
	while(i<63)
	{
		this->Name[i] = name[i];
		if(this->Name[i]=='\0')
			return;
		i++;
	}
	Name[63]='\0';
}

Transform*
IGObject::getTransform()
{
	return &This->transform;
}


IGObject::operator IConnectable(void)
{
	return *conXtor;
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
IMeshObject::draw()
{



	if(!IsVisible)
		return;
	
	if(!vertexBufferID)
		return;

	glEnable(GL_TEXTURE_2D);


	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	
	if(textureID)
	{
		glBindTexture(GL_TEXTURE_2D, textureID);
		glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
		glTexCoordPointer(2, GL_FLOAT, 0, 0);
	}
	else
		glColor4b(*this->color.R,*this->color.G,*this->color.B,*this->color.A);

	glPushMatrix();
	{
		
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
		glDrawArrays(this->FaceShape, 0, verts.size()-3);
	}
	glPopMatrix();
			
	glDisable(GL_TEXTURE_2D);

}

#undef This;


