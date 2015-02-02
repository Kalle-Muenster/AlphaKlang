#include "Kubus.h"

//

Kubus::Kubus(void)
{
	conXtor =  new IConXtor();
	conXtor->SetConnection(this);
	conXtor->SetName("last created");
	InitializeObject();

}

//#define this this->conXtor

Kubus::~Kubus(void)
{
}


void 
Kubus::InitializeObject(void)
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
Kubus::getTransform(void)
{

	return &this->transform;

}

void 
Kubus::draw(void)
{
	if(!primitiv->vertsBufferID)
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

	glBindBuffer(GL_ARRAY_BUFFER, primitiv->vertsBufferID);
	glVertexPointer(3, GL_FLOAT, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, primitiv->paintBufferID);
	glTexCoordPointer(2, GL_FLOAT, 0, 0);

	glPushMatrix();
	{
		Vector3 values = this->getTransform()->position;

		// Translation:
		glTranslatef(values.x, values.y, values.z);

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
		glDrawArrays(primitiv->shape, 0, primitiv->VertsCount);
	}
	glPopMatrix();

	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);
}




//#undef this