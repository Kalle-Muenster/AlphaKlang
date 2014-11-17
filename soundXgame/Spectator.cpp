#include "Spectator.h"

int Spectator::StaticCamModeID = -1;
float iX,Yps;
bool _willMove;

Spectator::Spectator(void)
{
	ModeName = "Spectator";
}

Spectator::~Spectator(void)
{

}



bool
Spectator::Initialize(void)
{
	angle		= 0;
	lx			= 0;
	lz			= -1;
	x			= 0;
	z			= 5;
	eyeY		= 1;
	moveSpeed	= 1.0f;
	mouseSpeed	= 0.75f;
	mouseX		= 0;
	mouseY		= 0;
	INPUT->attachKey(this);
	INPUT->attachMouseMove(this);
	INPUT->attachSpecial(this);
	StaticCamModeID = this->CamModeID();
	return true;
}

void 
Spectator::UpdateMode(void)
{ 

	camera->rotate(x+lx, eyeY, z+lz); 

	if(_willMove)
	{
		camera->transform.forward = (camera->transform.rotation - camera->transform.position).normal();
	//	camera->transform.forward = camera->transform.position.direction(camera->transform.rotation);
	//	camera->transform.right = Vector3(camera->transform.forward.z,camera->transform.forward.y,camera->transform.forward.x);
	//	camera->transform.up = Vector3(camera->transform.forward.x,camera->transform.forward.z,camera->transform.forward.y);

		camera->transform.position += camera->transform.forward * Yps;
		_willMove=false;
	}
	
	this->IsDirty=false;
}


void 
Spectator::keyPress(char key)
{
	float delta = moveSpeed * INPUT->FrameTime;
	iX=Yps=0;

	if(IsActive)
	{
		switch(key)
		{
			case 119: // W
				Yps = 1;
				break;
			case 115: // S
				Yps = -1;
				break;
			case 97: // A
				iX = -1;
				break;
			case 100: // D
				iX=1;
				break;
		}
		_willMove = Yps!=iX;

			Yps *= delta;
			iX  *= delta;
	}
}

void
Spectator::specialKeyPressed(int key)
{
	float delta = moveSpeed * INPUT->FrameTime;
	iX=0;
	Yps=0;

	if(IsActive)
	{
	switch (key) {
		case GLUT_KEY_UP:
			Yps=1;
			break;
		case GLUT_KEY_DOWN:
			Yps=-1;
			IsDirty=true;
			break;
		case GLUT_KEY_LEFT:
			iX=-1;
			break;
		case GLUT_KEY_RIGHT:
			iX=1;
			break;
		}
	iX*=delta;
	Yps*=delta;
	}
}

void
Spectator::mouseMotion(int newX, int newY)
{
	if(IsActive)
	{
		if(mouseX == 0 && mouseY == 0)
		{
			mouseX = newX;
			mouseY = newY;
		}
		
		int diffX = newX - mouseX;
		int diffY = newY - mouseY;

		angle += 0.005f * diffX * mouseSpeed;
		lx = sin(angle);
		lz = -cos(angle);
		eyeY += (float)diffY / 300;
		
		mouseX = newX;
		mouseY = newY;

		IsDirty=true;

		
	}
}


void
Spectator::mouseWheel(int wheel,WHEEL state)
{
	//moveSpeed += (float)state*0.01f;
}