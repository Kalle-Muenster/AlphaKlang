#include "FirstPerson.h"
#include "projectMacros.h"

int FirstPerson::StaticCamModeID = -1;


FirstPerson::FirstPerson(void)
{
	ModeName = "FirstPerson";
}

FirstPerson::~FirstPerson(void)
{

}



bool
FirstPerson::Initialize(void)
{
	angle		= 0;
	lx			= 0;
	lz			= -1;
	x			= 0;
	z			= 5;
	eyeY		= 1;
	moveSpeed	= 0.1f;
	mouseSpeed	= 1.0f;
	mouseX		= 0;
	mouseY		= 0;
	INPUT->attachKey(this);
	INPUT->attachMouseMove(this);
	INPUT->attachMouseWheel(this);
	INPUT->attachSpecial(this);
	StaticCamModeID = this->CamModeID();
	return true;
}

void 
FirstPerson::UpdateMode(void)
{ 
	camera->move(x, eyeY, z);
	camera->rotate(x+lx, 1.0f, z+lz); 
	this->IsDirty=false;
}


void 
FirstPerson::keyPress(char key)
{

	if(IsActive)
	{
		switch(key)
		{
			case 119: // W
				x += lx * moveSpeed;
				z += lz * moveSpeed;
				IsDirty=true;
				break;
			case 115: // S
				x -= lx * moveSpeed;
				z -= lz * moveSpeed;
				IsDirty=true;
				break;
			case 97: // A
				x += lz * moveSpeed;
				z += lx * (moveSpeed*-1);
				IsDirty=true;
				break;
			case 100: // D
				x -= lz * moveSpeed;
				z -= lx * (moveSpeed*-1);
				IsDirty=true;
				break;
		}
				

		UpdateMode();
	}
}

void
FirstPerson::specialKeyPressed(int key)
{
	if(IsActive)
	{
	switch (key) {
		case GLUT_KEY_UP:
			x += lx * moveSpeed;
			z += lz * moveSpeed;
			IsDirty=true;
			break;
		case GLUT_KEY_DOWN:
			x -= lx * moveSpeed;
			z -= lz * moveSpeed;
			IsDirty=true;
			break;
		case GLUT_KEY_LEFT:
			x += lz * moveSpeed;
			z += lx * (moveSpeed*-1);
			IsDirty=true;
			break;
		case GLUT_KEY_RIGHT:
			x -= lz * moveSpeed;
			z -= lx * (moveSpeed*-1);
			IsDirty=true;
			break;
		}
	UpdateMode();
	}
}

void
FirstPerson::mouseMotion(int newX, int newY)
{
	if(IsActive)
	{
		if(mouseX == 0 && mouseX == 0)
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

		UpdateMode();
	}
}

