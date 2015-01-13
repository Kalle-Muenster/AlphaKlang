#include "Spectator.h"


int Spectator::ID = -1;
float iX,Yps;


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
	
	delta=0;
	rotary3 = *Vector3::Zero;
	vec = *Vector3::Zero;
	ID = this->CamModeID();

	return true;
}

void 
Spectator::UpdateMode(void)
{ 
	//if(CamCubus == NULL)
	//	return;
	//else
	//{
	//camera->transform.position = CamCubus->getTransform()->position;
	//camera->transform.rotation = CamCubus->getTransform()->rotation;
	//}
	//if(INPUT->Mouse.WheelV!=WHEEL::NONE)
	//	delta = (float)INPUT->Mouse.WheelV/10 * INPUT->FrameTime;
	//if(INPUT->Mouse.MIDDLE.CLICK)
	//	delta = 0;

	//vec.x += INPUT->Mouse.Movement.x/1000;
	//vec.y += -INPUT->Mouse.Movement.y/1000;

	//glutSetCursor(GLUT_CURSOR_CYCLE);

	//rotary3.x = -sin(vec.x);
	//rotary3.z = cos(vec.x);
	//rotary3.y = sin(vec.y);

	//camera->transform.forward = rotary3;
	//camera->transform.up = rotary3 - Vector3(0,1,0);
	//camera->transform.right = rotary3.cros(camera->transform.up);
	//
	//rotary3 = camera->move(camera->transform.position+rotary3*delta);

//	gluLookAt(vec.x,vec.y,vec.z,rotary3.x, rotary3.y, rotary3.z, 0, 1,0);

}


void 
Spectator::keyPress(char key)
{
	float delta = camera->transform.speed * INPUT->FrameTime;
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
		cameraWillMove = Yps!=iX;

			Yps *= delta;
			iX  *= delta;
	}
}

void
Spectator::specialKeyPressed(int key)
{
	float delta = camera->transform.speed * INPUT->FrameTime;
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
	cameraWillMove = Yps!=iX;
	iX*=delta;
	Yps*=delta;
	}
}



//void
//Spectator::mouseWheel(int wheel,WHEEL state)
//{
//	Yps = (float)state * (moveSpeed * INPUT->FrameTime);
//	_willMove=true;
//}