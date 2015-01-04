#include "observer.h"
#include "projectMacros.h"
#include "CameraModesIncluder.h"

TargetGrabber::MODE _mode=TargetGrabber::MODE::OFF;
bool _firststart=true;
TargetGrabber::TargetGrabber(void)
{
	_targetGRABBED=false;
	ModeName = "TargetGrabber";
	isPrimarMode=false;

	targetConnectedMode[0]=targetConnectedMode[1]=targetConnectedMode[2]=NULL;
	targetSwitcher=1;
}


TargetGrabber::~TargetGrabber(void)
{

}

TargetGrabber::MODE
TargetGrabber::Mode(TargetGrabber::MODE value)
{
	if((value!=TargetGrabber::MODE::get)&&(value != _mode))
	{
		_mode=value;

		for(int i=0;i<3;i++)
			if(targetConnectedMode[i]!=EMPTY_SLOT)
				camera->GetTarget()->GetConnected<IConnectable>(targetConnectedMode[i])->IsActive=false;
	
		switch (value)
		{
		case MODE::MOVE:
			if(targetConnectedMode[0]!=EMPTY_SLOT)
				camera->GetTarget()->GetConnected<CamTargetMover>(targetConnectedMode[0])->IsActive=true;
			else
				camera->GetTarget()->AddConnectable<CamTargetMover>(&targetConnectedMode[0]);
			break;
		case MODE::ROTATE:
			if(targetConnectedMode[1]!=EMPTY_SLOT)
				camera->GetTarget()->GetConnected<CamTargetRotator>(targetConnectedMode[1])->IsActive=true;
			else
				camera->GetTarget()->AddConnectable<CamTargetRotator>(&targetConnectedMode[1]);
			break;
		case MODE::SCALE:
			if(targetConnectedMode[2]!=EMPTY_SLOT)
				camera->GetTarget()->GetConnected<CamTargetScaler>(targetConnectedMode[2])->IsActive=true;
			else
				camera->GetTarget()->AddConnectable<CamTargetScaler>(&targetConnectedMode[2]);
			break;
		}
		
		
		printf("CAMERA: TARGETGRABBER: Set mode: %s !\n",value==MODE::MOVE?"MOVE":value==MODE::ROTATE?"ROTATE":value==MODE::SCALE?"SCALE":"OFF");
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
		if(INPUT->Mouse.LEFT.DOUBLE)
		{
			ReleaseTarget();
			return;
		}

		if((!INPUT->Mouse.LEFT.HOLD)&&(INPUT->Mouse.WheelV!=WHEEL::NONE))
		{
			int mode=(int)_mode;
			mode+=(int)INPUT->Mouse.WheelV;
			if(mode<1)
				mode=3;
			else if(mode>3)
				mode=1;

			Mode((MODE)mode);
		}
	}
	else if(INPUT->Mouse.LEFT.DOUBLE)
	{
		printf("CAMERA: TARGETGRABBER: target \"%s\" - ID:%i grabbed!...\n",GrabTarget()->GetName(),camera->GetTarget()->GetID());
	}
	else if(INPUT->Mouse.WheelV!=WHEEL::NONE)
	{
		do{ if((targetSwitcher += INPUT->Mouse.WheelV) >= SCENE->HighestID())
				targetSwitcher = 1;
		else if(targetSwitcher < 1)
				targetSwitcher = SCENE->HighestID();
		} while(SCENE->Object(targetSwitcher)==NULL);

		SCENE->camera->SetTarget(SCENE->Object(targetSwitcher));
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
	if(!_firststart)
	{
		for(int i=0;i<3;i++)
		{
			camera->GetTarget()->Remove<IConnectable>(targetConnectedMode[i]);

			targetConnectedMode[i]=NULL;

		}
	}
	_firststart=false;
	std::cout<<"CAMERA: TARGETGRABBER: Target released!\n";
}






/////////////////////////////////
// toObject-connectable parts: //
/////////////////////////////////

/* CamTargetMover: */
CamTargetMover::CamTargetMover(void)
{
	INPUT->attachMouseMove(this);
}

bool
CamTargetMover::Initialize(void)
{
	return this->IsActive = this->Connection()!=NULL;
}

CamTargetMover::~CamTargetMover(void)
{
	INPUT->DetachMouseMove(this);
}

void
CamTargetMover::mouseMotion(int x,int y)
{
	if(SCENE->camera->mode<TargetGrabber>()->Mode()!=TargetGrabber::MODE::MOVE)
		IsActive=false;
	else if(IsActive)
	{
			if(SCENE->camera->GetTarget() == this->Connection())
			{
				float distance = SCENE->camera->GetTargetDistance();
				Vector3 targetDestination =  SCENE->camera->transform.position.direction(SCENE->camera->transform.rotation);
				
				this->Connection()->move((SCENE->camera->transform.position + (targetDestination * distance)));
			}
			else
				INPUT->DetachMouseMove(this);
	}
}


////////////////////////////////////////

/* CamTargetRotator: */
CamTargetRotator::CamTargetRotator(void)
{
	IsActive=true;
}

CamTargetRotator::~CamTargetRotator(void)
{
	UPDATE->SignOutFromUpdate(this);
}

void
CamTargetRotator::RotateOnMouseHold(void)
{
		Vector3 temp = this->Connection()->getTransform()->rotation;
		temp.y += INPUT->Mouse.Movement.x;
		temp.z += INPUT->Mouse.Movement.y;

		this->Connection()->rotate(temp.x,temp.y,temp.z);
}

void
CamTargetRotator::DoUpdate(void)
{
	if(SCENE->camera->mode<TargetGrabber>()->Mode()!=TargetGrabber::MODE::ROTATE)
		IsActive=false;
	else if(SCENE->camera->GetTarget()->GetID() == this->Connection()->GetID())
	{
		if(INPUT->Mouse.MIDDLE.HOLD)
		{
			RotateOnMouseHold();
		}
		return;
	}
	else
		UPDATE->SignOutFromUpdate(this);

	IsActive=false;
}



//////////////////////////////////////

/* CamTargetScaler: */
CamTargetScaler::CamTargetScaler(void)
{
	IsActive=false;	
}


CamTargetScaler::~CamTargetScaler(void)
{
	UPDATE->SignOutFromUpdate(this);
}

bool 
CamTargetScaler::Initialize(void)
{
	vec = *Vector3::Zero; 
	return IsActive = true;
}

void 
CamTargetScaler::DoUpdate(void)
{
	if(SCENE->camera->mode<TargetGrabber>()->Mode() != TargetGrabber::MODE::SCALE)
		IsActive=false;
	else if(IsActive)
	{
		if(SCENE->camera->GetTarget() == this->Connection())
		{
			vec = this->Connection()->getTransform()->scale;
			
			if(INPUT->Mouse.LEFT.HOLD && INPUT->Mouse.RIGHT.HOLD)
				vec.z *= (1.f-(INPUT->Mouse.Movement.y/100));
			else if(INPUT->Mouse.LEFT.HOLD)
				vec.x *= (1.f+(INPUT->Mouse.Movement.x/100));
			else if(INPUT->Mouse.MIDDLE.HOLD)
				vec.y *= (1.f-(INPUT->Mouse.Movement.y/100));
			else 
			{
				float scalar = (1.f-(INPUT->Mouse.Movement.y/100));
				vec.x *= scalar;
				vec.y *= scalar;
				vec.z *= scalar;
			}
			this->Connection()->scale(vec);
		}
		else
			UPDATE->SignOutFromUpdate(this);
	}
}
