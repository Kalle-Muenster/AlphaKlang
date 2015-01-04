#include "VoxControl.h"
#include "VoxGrid.h"
#include "CameraModesIncluder.h"


string
VoxControl::files[6];


VoxControl::VoxControl(void)
{
	bumper = 0.0000f;
	timer = 0;
	tempvector = *Vector3::Zero;
	bumpmapchannel=0;
		currentSellection = 0;
		files[0]="Bump_6.ppm";
		files[1]="Bump_5.ppm";
		files[2]="Bump_3.ppm";
		files[3]="Color_1.ppm";
		files[4]="Color_3.ppm";
		files[5]="Color_4.ppm";
		bumpmapchannel=0;
		bumpmapIndex=0;
		imageIndex=0;
		ZedMode = false;
		YPS=0.01;

	  TypeHashCode=(unsigned)typeid(VoxControl).hash_code();

}


VoxControl::~VoxControl(void)
{
}


bool 
VoxControl::Initialize(void)
{
		INPUT->attachKey(this);

		return true;
}


void 
VoxControl::keyPress(char key)
{
	if(SceneGraph::getInstance()->camera->GetTarget()==this->Connection())
	{
	if(key=='x')
	{
		if(lastKey!='x')
		{
			if(++currentSellection>=5)
				currentSellection=0;
			printf("File \"%s\" Sellected...\n",files[currentSellection]);
			lastKey='x';
		}
	}


	if(key == 'r')
	{
		if(lastKey!='r')
		{
			this->vConnection()->ReLoade(files[imageIndex=currentSellection]);
			printf("Image \"%s\" geloadedt...\n",files[currentSellection]);
			vConnection()->MainSizzes.x=0.2;
			vConnection()->MainSizzes.y=0.045f;
			lastKey='r';
		}
	}


	if(key == 'l')
	{
		if(lastKey!='l')
		{
			this->vConnection()->LoadeMap(files[bumpmapIndex=currentSellection],bumpmapchannel);
		printf("BumpMap file-\"%s\"-Channel %i geloadedt !\n",files[currentSellection],bumpmapchannel);
		if(++bumpmapchannel>3)
			bumpmapchannel=0;
		lastKey='l';
		}
	}


	if(key=='b')
		this->vConnection()->Mode(VoxGrid::ColorMode::BUNT);

	if(key=='n')
		this->vConnection()->Mode(VoxGrid::ColorMode::NORMAL);

	if(key=='h')
		this->vConnection()->Mode(VoxGrid::ColorMode::BYTE);

	if(key=='v')
	{
		if(lastKey!='v')
		{
			SCENE->camera->SetTarget(this->Connection());
			lastKey='v';
		}
	}

	if(key=='t')
	{
		if(lastKey!='t')
		{
			if(transparenseEnabled)
			{
				glDisable(GL_BLEND);
			}
			else
			{
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}
			transparenseEnabled = !transparenseEnabled;
			lastKey='t';
		}
	}

	if(key=='u')
	{
		if(lastKey!='u')
		{
			this->vConnection()->flip();
			lastKey='u';
		}
	}

	if(key=='z')
		ZedMode=true;
	else
		ZedMode=false;
	}
}





VoxGrid* 
VoxControl::vConnection(void)
{
	return (VoxGrid*)connection; 
}



void
VoxControl::DoUpdate(void)
{
	if(lastKey!='\0')
	{
		timer+=INPUT->FrameTime;
		if(timer>0.5)
		{
			timer=0;
			lastKey='\0';
		}
	}
	if(SceneGraph::getInstance()->camera->GetTarget()==this->Connection())
	{
		//if(SCENE->camera->mode<TargetGrabber>()->IsATargetGrabbed())
		//	SCENE->camera->ModeSocket->GetCameraMode<TargetGrabber>()->Mode(TargetGrabber::MODE::OFF);
		
		if(INPUT->Mouse.LEFT.HOLD)
		{
			this->vConnection()->MainSizzes.x *= 1-INPUT->Mouse.Movement.x/300;
			if(!ZedMode)this->vConnection()->MainSizzes.y *= 1+INPUT->Mouse.Movement.y/600;
			else this->vConnection()->move(0,0, INPUT->Mouse.Movement.y);
			printf("MainSizerX: %f ,MainSizerY: %f\n",this->vConnection()->MainSizzes.x,this->vConnection()->MainSizzes.y);
		}
		if(INPUT->Mouse.MIDDLE.HOLD)
		{
			tempvector = this->vConnection()->getTransform()->rotation;
			vConnection()->getTransform()->position.z = ( tempvector.z += INPUT->Mouse.Position.x);
			tempvector.y += INPUT->Mouse.Movement.y;
			this->vConnection()->rotate(tempvector.x,tempvector.y,tempvector.z);
		}

		if(INPUT->Mouse.WheelV==WHEEL::UP)
		{
			if(INPUT->Mouse.LEFT.HOLD)
				bumper=0.00001;
			SCENE->camera->transform.position += Vector3(0,YPS,0);
		}
	else 
		if(INPUT->Mouse.WheelV==WHEEL::DOWN)
		{
			if(INPUT->Mouse.LEFT.HOLD)
				bumper= -0.00001;
			SCENE->camera->transform.position -= Vector3(0,YPS,0);
		}

		vConnection()->BumpFactor += bumper;
	}
}