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

void 
TargetGrabber::UpdateMode(void)
{
	if(_targetGRABBED)
	{
		int mode=(int)_mode;
		mode+=(int)INPUT->Mouse.WheelV;
		if(mode<0)
			mode=3;
		else if(mode>3)
			mode=0;

		Mode((MODE)mode);

		Vector3 temp;
		_targetGRABBED = !INPUT->Mouse.LEFT.DOUBLE;
		switch(Mode())
		{
		case MODE::MOVE:
			camera->GetTarget()->getTransform()->position = (camera->transform.position + (camera->transform.forward * camera->GetTargetDistance())) + (INPUT->Mouse.MIDDLE.HOLD? (camera->transform.forward * -INPUT->Mouse.Movement.y/10) : Vector3(0,0,0));
			break;

		case MODE::ROTATE:
			temp = camera->GetTarget()->getTransform()->rotation;
			if(INPUT->Mouse.MIDDLE.HOLD)
				temp.z += INPUT->Mouse.Movement.y;
			else
				temp.x += INPUT->Mouse.Movement.y;
			temp.y += INPUT->Mouse.Movement.x;
			camera->GetTarget()->rotate(temp);
			break;

		case MODE::SCALE:
			temp = camera->GetTarget()->getTransform()->scale;
			
			if(INPUT->Mouse.LEFT.HOLD)
			{
				temp.x+=INPUT->Mouse.Movement.x;
				temp.z+=INPUT->Mouse.Movement.y;
			}
			else if(INPUT->Mouse.MIDDLE.HOLD)
			{
				temp.x+=INPUT->Mouse.Movement.x;
				temp.y+=INPUT->Mouse.Movement.y;
			}
			else
			{
				temp.x+=INPUT->Mouse.Movement.y;
				temp.y+=INPUT->Mouse.Movement.y;
				temp.z+=INPUT->Mouse.Movement.y;
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