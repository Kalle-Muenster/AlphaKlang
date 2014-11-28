#include "TargetGrabber.h"
#include "CameraModesIncluder.h"



TargetGrabber::TargetGrabber(void)
{
	_targetGRABBED=false;
	ModeName = "TargetGrabber";
	isPrimarMode=false;
	_mode=MODE::ROTATE;
}


TargetGrabber::~TargetGrabber(void)
{

}

TargetGrabber::MODE
TargetGrabber::Mode(TargetGrabber::MODE value)
{
	if((value!=TargetGrabber::MODE::get)&&(value!=_mode))
	{
		_mode=value;
		printf("CAMERA: TARGETGRABBER: Set mode: %s !\n",_mode==MODE::MOVE?"MOVE":_mode==MODE::ROTATE?"ROTATE":_mode==MODE::SCALE?"SCALE":"OFF");
	}
	return _mode;
}

int 
TargetGrabber::StaticCamModeID = -1;

bool
TargetGrabber::Initialize(void)
{
	StaticCamModeID = this->CamModeID();
	return true;
}

bool
TargetGrabber::IsATargetGrabbed(void)
{
	return this->IsActive && this->_targetGRABBED;
}

void 
TargetGrabber::UpdateMode(void)
{
	if(_targetGRABBED)
	{
		_targetGRABBED = !INPUT->Mouse.LEFT.DOUBLE;
		if(!_targetGRABBED)
		{
			std::cout<<"CAMERA: TARGETGRABBER: Target released!\n";
			return;
		}

		if(INPUT->Mouse.WheelV!=WHEEL::NONE)
		{
			int mode=(int)_mode;
			mode+=(int)INPUT->Mouse.WheelV;
			if(mode<1)
				mode=3;
			else if(mode>3)
				mode=1;

			Mode((MODE)mode);
		}

		Vector3 temp;
		switch(Mode())
		{
		case MODE::MOVE:
			camera->GetTarget()->getTransform()->position = (camera->transform.position + (camera->transform.forward * camera->GetTargetDistance()));
			if(INPUT->Mouse.LEFT.HOLD)
				camera->GetTarget()->getTransform()->position += (camera->transform.forward.normal() * INPUT->Mouse.Movement.y/10);
			break;

		case MODE::ROTATE:
			temp = camera->GetTarget()->getTransform()->rotation;
			if(INPUT->Mouse.MIDDLE.HOLD)
				temp.z += INPUT->Mouse.Movement.y/5;
			else
				temp.x += INPUT->Mouse.Movement.y/5;
			temp.y += INPUT->Mouse.Movement.x/5;
			camera->GetTarget()->rotate(temp);
			break;

		case MODE::SCALE:
			temp = camera->GetTarget()->getTransform()->scale;
			float yps = INPUT->Mouse.Movement.y/10;
			float ix = INPUT->Mouse.Movement.x/10;
			if(INPUT->Mouse.LEFT.HOLD)
			{
				temp.x+=ix;
				temp.z+=yps;
			}
			else if(INPUT->Mouse.MIDDLE.HOLD)
			{
				temp.x+=ix;
				temp.y+=yps;
			}
			else
			{
				temp.x+=yps;
				temp.y+=yps;
				temp.z+=yps;
				camera->GetTarget()->getTransform()->position = (camera->transform.position + (camera->transform.forward * camera->GetTargetDistance()));
			}

			camera->GetTarget()->scale(temp);

			break;
		}
	
	}
	else if(INPUT->Mouse.LEFT.DOUBLE)
	{
		printf("CAMERA: TARGETGRABBER: target \"%s\" - ID:%i grabbed!...\n",GrabTarget()->GetName(),camera->GetTarget()->GetID());
	}
}

IGObject*
TargetGrabber::GrabTarget(void)
{
	_targetGRABBED = camera->GetTarget()!=NULL;
	return camera->GetTarget();
}

void
TargetGrabber::ReleaseTarget(void)
{
	_targetGRABBED=false;
	Mode(MODE::OFF);
	std::cout<<"CAMERA: TARGETGRABBER: Target released!\n";
}