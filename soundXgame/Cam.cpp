#include "Cam.h"
#include "projectMacros.h"

static bool _shareAudioReciever = true;

Cam::Cam(void)
{
	_isFollowingTarget=false;
	this->transform.position.x=0;
	this->transform.position.y=0;
	this->transform.position.z=0;
	this->transform.rotation.x=0;
	this->transform.rotation.y=0;
	this->transform.rotation.z=0;
	this->transform.movement.x=0;
	this->transform.movement.y=0;
	this->transform.movement.z=0;


	InitiateListener(&this->transform);

	_fieldOfView = 55;
	_aspect = 16.0/9.0;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(transform.position.x, transform.position.y, transform.position.z, transform.rotation.x,transform.rotation.y,transform.rotation.z, 0, 1, 0);
	_mode = CAM_MODE::PERSPECTIVE;
}

volatile bool
Cam::IsShared(bool setter)
{
	if(setter!=NULL)
	{
		if(setter && !_shareAudioReciever)
			GetMasterReciever()==this;
		_shareAudioReciever = setter;
	}
	return _shareAudioReciever;
}

bool
Cam::ShareAudio(BOOL setter)
{
	if(setter<3)
		_shareAudioReciever=setter;
	return _shareAudioReciever;
}

Cam::~Cam(void)
{
	_FollowFirstPerson=false;
	delete camTarget;
}

void
Cam::SetTarget(AbsGobject *targetObject)
{
	_distanceToTarget = 0.1;
	this->camTarget = &targetObject->getTransform()->position;
	if (this->camTarget)
		_isFollowingTarget = true;
	_target = targetObject;
	_target->IsVisible=true;
	_FollowFirstPerson=false;
	printf("\nCAMERA: Set %s-ID:%i As Follow-Target!",_target->GetName(),_target->GetObjectID());
}

AbsGobject* 
Cam::GetTarget(void)
{
	if(_isFollowingTarget||_FollowFirstPerson)
	return _target;
	else
	return NULL;
}

void
Cam::followTarget()
{
	if (this->camTarget)
	{
		_isFollowingTarget = true;
		_FollowFirstPerson = false;
		_target->IsVisible=true;
	}
}

void
Cam::StopFollowing()
{
	_FollowFirstPerson = _isFollowingTarget = false;
	this->GetTarget()->IsVisible=true;
	_target->IsVisible=true;
}

void
Cam::SetFirstPerson(IGobject* personObj)
{
	_target->IsVisible=true;
	_target = personObj;
	_target->IsVisible=false;
	this->camTarget = &personObj->getTransform()->position;
	_isFollowingTarget=false;
	_FollowFirstPerson=true;
}

ConID*
Cam::SetFirstPerson(IConnectable* onPerson)
{
	this->targetConID = this->ConnectConnectableInstance(onPerson);
	this->camTarget = &GetConnected<IConnectable>(*targetConID)->Connection()->getTransform()->position;
	_target->IsVisible=true;
	_target = GetConnected<IConnectable>(*targetConID)->Connection();
	_isFollowingTarget=false;
	_FollowFirstPerson=true;
	return targetConID;
	_target->IsVisible=false;
}

void
Cam::SetTargetasFirstPerson(void)
{
	_target->IsVisible=false;
	_FollowFirstPerson=true;
	_isFollowingTarget=false;
}

const
glm::vec3
Cam::GetTargetPosition()
{
	return *this->camTarget;
}

//void 
//Cam::keyPress(char key)
//{
//
//}
//
//void
//Cam::mouseMotion(int x,int y)
//{
//
//}
//
//void
//Cam::specialKeyPressed(int spezial)
//{
//
//}
void 
Cam::WheelVRoll(WHEEL state)
{
	transform.position.y-=(float)state/10;
	FieldOfView(FieldOfView() - state); 
}

BASS_3DVECTOR
Cam::move(glm::vec3  newPosition)
{
		this->transform.movement.x = newPosition.x - this->transform.position.x;
		this->transform.movement.y = newPosition.y - this->transform.position.y;
		this->transform.movement.z = newPosition.z - this->transform.position.z;

		this->transform.position.x = newPosition.x;
		this->transform.position.y = newPosition.y;
		this->transform.position.z = newPosition.z;



		SetMyPosition(&this->transform);

		if(transform.movement.x!=0 || transform.movement.y!=0 || transform.movement.z!=0)
			printf("\nCAMERA: moved to: %f,%f,%f !",transform.position.x,transform.position.y,transform.position.z);

		return transform.position;
}

BASS_3DVECTOR
Cam::rotate(glm::vec3 newRotation)
{
		this->transform.rotation.x = newRotation.x;
		this->transform.rotation.y = newRotation.y;
		this->transform.rotation.z = newRotation.z;

		this->transform.forward = glm::normalize((glm::vec3)this->transform.rotation);
		
		return this->transform.rotation;
}

double
Cam::FieldOfView(double setter)
{
	if(setter!=_FNan._Double)
	{
		_fieldOfView = setter;
		UpdateView();
	}
	return _fieldOfView;
}

GLfloat
Cam::Aspect(GLfloat aspect)
{
	if(aspect)
	{
		_aspect = aspect;
		UpdateView();
	}
	return _aspect;
}

CAM_MODE
Cam::Mode(CAM_MODE mode)
{
	if(mode)
	{
		_mode=mode;
		UpdateView();
		printf("\nCAMERA: Set to %s-Mode !",_mode==ORTHOGRAFIC?"Orthografic":_mode==PERSPECTIVE?"Perspective":NULL);
	}
	return _mode;
}

void
Cam::UpdateView(void)
{
	glViewport(0, 0, INPUT->GetViewportRectangle().w,INPUT->GetViewportRectangle().z);
	
	if(Mode()==PERSPECTIVE)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluPerspective(FieldOfView(), Aspect(), 0.1f, 100.0f);
	}

	if(Mode()==ORTHOGRAFIC)
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluOrtho2D(0,SCREENWIDTH,SCREENHEIGHT,0);
	
		gluLookAt(0,0,0,0,0,0,0,1,0);
		
	}
}

void
Cam::Update()
{
	IsShared(this->ShareAudio());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(_isFollowingTarget)
	{
		this->move(transform.position);
		this->rotate(*camTarget);
	}
	if(_FollowFirstPerson)
	{
		this->move(*camTarget);
		this->rotate(_target->getTransform()->rotation);
	}

	if(INPUT->Mouse.MIDDLE.HOLD)
		this->move(transform.position + Vector3(INPUT->Mouse.Movement.x/100,0,INPUT->Mouse.Movement.y/100));

	//Set The Position of the attached AudioListener
	SetMyPosition(&transform);

	if(Mode()==PERSPECTIVE)
		gluLookAt(transform.position.x, transform.position.y, transform.position.z, transform.rotation.x,transform.rotation.y,transform.rotation.z, 0, 1, 0);
//	else if(Mode()==ORTHOGRAFIC)
//		gluLookAt(0,0,0,0,0,0,0,1,0);
}

