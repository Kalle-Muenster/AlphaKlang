#include "ShaderObj.h"
#include "projectMacros.h"

ShaderObj::ShaderObj(void)
{
	//InitializeObject("cube_quads.obi",false);
	//this->LoadTexture("X-512.jpg");
	
	InitializeObject();

	UpdateManager::getInstance()->SignInForUpdate(this);
}

ShaderObj::~ShaderObj(void)
{
}

void
ShaderObj::DoUpdate(void)
{

}

void
ShaderObj::InitializeObject(void)
{
	SetID(SCENE->Add(this));

	LockID();
}

void
ShaderObj::draw(void)
{
	if(!IsVisible)
		return;
	
	this->drawShader();

	/*
	if(!vertexBufferID)
		return;

	if(NoBackfaceCulling)
		glDisable(GL_CULL_FACE);
	else
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	if(UseTexture)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture.ID);
	}
	else
		glDisable(GL_TEXTURE_2D);
		
	glColor4b(color.byte[1],color.byte[2],color.byte[3],color.byte[0]);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferID);
	glVertexPointer(3, GL_FLOAT, 0, 0);
	
	if(UseTexture)
	{
		glBindBuffer(GL_ARRAY_BUFFER, uvBufferID);
		glTexCoordPointer(2, GL_FLOAT, 0, 0);
	}

	glPushMatrix();
	{
		//Translate...
		GLfloat iX  = getTransform()->position.x;
		GLfloat Zed = getTransform()->position.z;
		GLfloat Yps = IsGrounded? Ground::getInstance()->GetGroundY(iX, Zed) : getTransform()->position.y;
		glTranslatef(iX, Yps, Zed);

		//Rotate...
		glRotatef(getTransform()->rotation.x, 1, 0, 0);
		glRotatef(getTransform()->rotation.y, 0, 1, 0);
		glRotatef(getTransform()->rotation.z, 0, 0, 1);

		//Scaleate...
		glScalef(getTransform()->scale.x,getTransform()->scale.y,getTransform()->scale.z);
		
		//Draw
		glDrawArrays(FaceShape, 0, verts.size());
	}
	glPopMatrix();
	*/
}