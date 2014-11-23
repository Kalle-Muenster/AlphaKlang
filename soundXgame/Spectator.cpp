#include "Spectator.h"


int Spectator::StaticCamModeID = -1;
float iX,Yps;
bool _willMove;
bool _resetCursor = false;
float _xmover,_ymover;

Spectator::Spectator(void)
{
	ModeName = "Spectator";
	isPrimarMode=true;
}

Spectator::~Spectator(void)
{

}



bool
Spectator::Initialize(void)
{
	_xmover=_ymover=0;
	StaticCamModeID = this->CamModeID();
	return true;
}

void 
Spectator::UpdateMode(void)
{ 
	_xmover+=INPUT->Mouse.Movement.y;
	_ymover+=INPUT->Mouse.Movement.x;

	Vector3 direction = (camera->transform.forward + (camera->transform.right * (INPUT->Mouse.Movement.x)) + (camera->transform.up *(INPUT->Mouse.Movement.y)));

	camera->rotate(direction.normal());
	//	camera->transform.forward = (camera->transform.rotation - camera->transform.position).normal();
	if(_resetCursor)
	//	glutWarpPointer(SCREENWIDTH/2,SCREENHEIGHT/2);

	if(_willMove)
	{
//		camera->transform.forward = (camera->transform.rotation - camera->transform.position).normal();
		camera->transform.position += camera->transform.forward * Yps;
		_willMove=false;
	}
	
	_resetCursor=false;
	this->IsDirty=false;
	glPushMatrix();
	glTranslatef(camera->transform.position.x,camera->transform.position.y,camera->transform.position.z);
	glRotatef(camera->transform.rotation.x,1,0,0);
	glRotatef(camera->transform.rotation.y,0,1,0);
	glRotatef(camera->transform.rotation.z,0,0,1);

	gluLookAt(0, 0, 0,
	0,0,-1,	0, 1, 0);
	glPopMatrix();
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
	_willMove = Yps!=iX;
	iX*=delta;
	Yps*=delta;
	}
}

void
Spectator::mouseMotion(int newX, int newY)
{
	if(IsActive)
	{

	}
}


void
Spectator::mouseWheel(int wheel,WHEEL state)
{
	Yps = (float)state * (moveSpeed * INPUT->FrameTime);
	_willMove=true;
}