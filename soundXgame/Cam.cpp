#include "projectMacros.h"
#include "Ground.h"
#include "CameraModesIncluder.h"
#include "Cam.h"
 //#define DEBUG_OUTPUT_CAMERA

#define MAXIMUM_NUMBER_OF_CAMERA_MODES  20

bool 
Cam::_shareAudioReciever = true;


Cam::Cam(void) :
	NumberOfModes(0),
	_mode(1)
{
	this->transform.position.x=0;
	this->transform.position.y=1;
	this->transform.position.z=5;
	this->transform.rotation.x=0;
	this->transform.rotation.y=0;
	this->transform.rotation.z=0;
	this->transform.movement.x=0;
	this->transform.movement.y=0;
	this->transform.movement.z=0;

	InitiateListener(&this->transform);

	_fieldOfView = 55;
	_aspect = 16.0f/9.0f;

	ModeSocket = new CameraMode();
	ModeSocket->camera = this;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(transform.position.x, transform.position.y, transform.position.z, transform.rotation.x,transform.rotation.y,transform.rotation.z, 0, 1, 0);
	
	ModeSocket->AddCameraMode<FollowTarget>()->IsActive=false;
	ModeSocket->AddCameraMode<FirstPerson>()->IsActive=true;
	ModeSocket->AddCameraMode<Spectator>()->IsActive=false;
	ModeSocket->AddCameraMode<TargetGrabber>()->IsActive=true;
	
}


bool
Cam::IsShared(bool setter)
{
	if(setter!=NULL)
	{
		if(setter && !_shareAudioReciever)
			MasterReciever=this;
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
	delete _targetPosition;
	delete _targetObject;
	delete ModeSocket;
	delete IAudioReciever::MasterReciever;
}

void
Cam::SetTarget(Vector3 *position)
{
	_distanceToTarget = transform.position.distance(*position);
	_targetPosition = position;
}

IGObject*
Cam::SetTarget(IGObject *targetObject)
{
	//Mode(FOLLOWTARGET);
	_distanceToTarget = transform.position.distance(targetObject->getTransform()->position);
	_targetPosition = &targetObject->getTransform()->position;
	if(_targetObject!=targetObject)
		ModeSocket->GetCameraMode<TargetGrabber>()->ReleaseTarget();
	_targetObject = targetObject;
	_targetObject->IsVisible=true;
	
	printf("CAMERA: Set %s-ID:%i As Camera-Target!\n",_targetObject->GetName(),_targetObject->GetID());
	return _targetObject;
}

IGObject* 
Cam::GetTarget(void)
{
	return _targetObject;
}

void
Cam::followTarget(void)
{
	if(this->_targetPosition)
		Mode(FOLLOWTARGET);
}

void
Cam::stopFollowing(void)
{

}

float
Cam::GetTargetDistance(void)
{
	
	if(_targetPosition!=NULL)
	{
		if(TransformDIRTY)
			_distanceToTarget = transform.position.distance(GetTargetPosition());
		return _distanceToTarget;
	}
	else
		return 0;
}

Vector3		
Cam::GetTargetPosition(void)
{
	if(_targetPosition)
		return *_targetPosition;
	else
		return transform.position;
}






Vector3		
Cam::move(float x,float y,float z)
{
	return move(Vector3(x,y,z));
}

Vector3		
Cam::move(glm::vec3  newPosition)
{
	if(transform.position != newPosition)
	{
		this->transform.movement.x = newPosition.x - this->transform.position.x;
		this->transform.movement.y = newPosition.y - this->transform.position.y;
		this->transform.movement.z = newPosition.z - this->transform.position.z;
		this->transform.speed = transform.movement.GetLength();

		this->transform.position.x = newPosition.x;
		this->transform.position.y = newPosition.y;
		this->transform.position.z = newPosition.z;

		TransformDIRTY = true;

		//#ifdef DEBUG_OUTPUT_CAMERA
		//	printf("CAMERA: moved to: %f,%f,%f !\n",transform.position.x,transform.position.y,transform.position.z);
		//#endif
	}
	return transform.position;
}

Vector3		
Cam::rotate(float x,float y,float z)
{
	if(transform.rotation.x != x)
		{transform.rotation.x=x; TransformDIRTY=true;}
	if(transform.rotation.y != y)
		{transform.rotation.y=y; TransformDIRTY=true;}
	if(transform.rotation.z != z)
		{transform.rotation.z=z; TransformDIRTY=true;}
	if(TransformDIRTY)
		UpdateDirections();
	return transform.rotation;
}

Vector3		
Cam::rotate(Vector3 newRotation)
{
	return rotate(newRotation.x,newRotation.y,newRotation.z);
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
Cam::Mode(CAM_MODE value)
{
//get():
	if(value==set)
		return (CAM_MODE)_mode;
//set(value):
	if(value != (CAM_MODE)_mode)
	{
		if(ModeSocket->Get<CameraMode>(value)->IsPrimarMode())
		{
			ModeSocket->Get<CameraMode>(_mode)->IsActive = false;
			ModeSocket->Get<CameraMode>(value)->IsActive = true;
			_mode = value;
		}
		else
		{
			ModeSocket->Get<CameraMode>(value)->IsActive = true;
		}
		
		printf("CAMERA: Mode: %s activated! !\n", ModeSocket->Get<CameraMode>(value)->ModeName);
		
		UpdateView();
	}
}


void // UpdateView: its called on GL-Reshape (if window has been resized...)
Cam::UpdateView(void)
{
	glViewport(0, 0, INPUT->GetViewportRectangle()->size().x,INPUT->GetViewportRectangle()->size().y);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FieldOfView(), Aspect(), 0.1f, 1000.0f);
	TransformDIRTY = true;
}

void//recalculates the direction-vectors...
Cam::UpdateDirections(void)
{
	if(TransformDIRTY)
	{
	//	transform.forward = (transform.rotation - transform.position);
		//transform.right = ( transform.forward - Vector3(0,0,1) + Vector3(0,1,0));
	//	transform.up = (transform.forward - Vector3(0,0,1) + Vector3(1,0,0));
	}
}


/* The Camera's Update-function: */ 
//it's called by the SCENE just before it's MainDrawCycle each frame...
void 
Cam::Update()
{
	IsShared(this->ShareAudio());

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	if(Mode()==FOLLOWTARGET)
	{
		this->move(transform.position);
		this->rotate(*_targetPosition);
	}

	//Mode-Dependant updates...
	//ModeSocket->UpdateAllActiveModes();
	
	//update camera position:
	UpdateDirections();
	
	//Mode-Dependant updates...
	ModeSocket->UpdateAllActiveModes();


	//Set To GL
	//gluLookAt(transform.position.x, transform.position.y, transform.position.z,
	//transform.rotation.x,transform.rotation.y,transform.rotation.z,	0, 1, 0);



	//update The Position of the attached AudioReciever
	SetAudioResieverPosition(&transform);
	TransformDIRTY = false;

}

/* * * * User Input * * * */


void
Cam::WheelVRoll(WHEEL state)
{
	
}


