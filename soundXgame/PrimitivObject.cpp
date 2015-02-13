#include "PrimitivObject.h"
#include "Ground.h"

IPrimitivObject::IPrimitivObject(void)
{
	conXtor =  new IConXtor();
	conXtor->SetConnection(this);
	conXtor->SetName("last created");
	InitializeObject();
	//SetPrimitiv<IBall>();
}


IPrimitivObject::~IPrimitivObject(void)
{
}


void 
IPrimitivObject::InitializeObject(void)
{
	conXtor->AddToSceneAndLockID();
	color.byte[1] = 32;
	color.byte[2] = 128;
	color.byte[3] = 255;		  
	color.byte[0] = 128;
	UseTexture(false);
	conXtor->IsActive=true;
}


Transform*
IPrimitivObject::getTransform(void)
{

	return &this->transform;

}

void 
IPrimitivObject::draw(void)
{
	if(!pri.vertsBufferID)
		return;

	if(NoBackfaceCulling == false)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	if(UseTexture())
	{
		glColor3f(1,1,1);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture.ID);
	}
	else
	{
		glColor4b(Connection()->color.byte[1],Connection()->color.byte[2],Connection()->color.byte[3],Connection()->color.byte[0]);
	}

	glBindBuffer(GL_ARRAY_BUFFER, pri.vertsBufferID);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, pri.paintBufferID);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);

	glPushMatrix();
	{
		Vector3 values = this->getTransform()->position;

		// Translation:
		glTranslatef(values.x, values.y, values.z);

		//Grounding:
		if(IsGrounded())
		{
			float y = Ground::getInstance()->GetGroundY(values.x, values.z);
			glTranslatef(0, y, 0);
		}

		// Rotation:
		values = this->getTransform()->rotation;

		if(this->angle==0) //by the transform's rotation-values as angles at their coresponding global-axis...
		{
			glRotatef(values.x, 1, 0, 0);
			glRotatef(values.y, 0, 1, 0);
			glRotatef(values.z, 0, 0, -1);
		}
		else // if angle is not zero, rotation will be aplied as rotation around an axis wich's defined by the transform's rotation-values.
			glRotatef(angle,values.x,values.y,values.z);

		// Scaling:
		values = this->getTransform()->scale;
		glScalef(values.x,values.y,values.z);

		// Draw
		glDrawArrays(pri.shape, 0, pri.VertsCount);
	}
	glPopMatrix();

	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
}




