#include "Cam.h"
#include "projectMacros.h"
#include "Ground.h"
#include "CameraModesIncluder.h"

 //#define DEBUG_OUTPUT_CAMERA

#define MAXIMUM_NUMBER_OF_CAMERA_MODES  20
static unsigned _modeID = 0;
static Cam* _Socket;
static bool _shareAudioReciever = true;


int _spectator = -1;

Cam::Cam(void) :
	NumberOfCameraModes(0)
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
	_aspect = 16.0/9.0;

	ModeSocket = new CameraMode();
	ModeSocket->camera = this;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(transform.position.x, transform.position.y, transform.position.z, transform.rotation.x,transform.rotation.y,transform.rotation.z, 0, 1, 0);

	ModeSocket->AddCameraMode<TargetGrabber>()->IsActive=true;
	ModeSocket->AddCameraMode<Spectator>()->IsActive=false;
	ModeSocket->AddCameraMode<FirstPerson>()->IsActive=true;
	INPUT->attachMouseWheel(this);
}


bool
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
	delete _targetPosition;
	delete _targetObject;
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
	Mode(FOLLOWTARGET);
	_distanceToTarget = transform.position.distance(targetObject->getTransform()->position);
	_targetPosition = &targetObject->getTransform()->position;
	_targetObject = targetObject;
	_targetObject->IsVisible=true;
	
	printf("CAMERA: FOLLOWTARGET: Set %s-ID:%i As Follow-Target!\n",_targetObject->GetName(),_targetObject->GetID());
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
	Mode(CAM_MODE_NULL);
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




/* * * * Transform * * * */

// -> now in ModeUpdates...
//void
//Cam::UpdateTransform(void)
//{
//	this->move(x, eyeY, z);
//	this->rotate(x+lx, 1.0f, z+lz);
//	this->_transformChanged = false;
//}

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
	return rotate(Vector3(x,y,z));
}

Vector3		
Cam::rotate(glm::vec3 newRotation)
{
	if(transform.rotation!=newRotation)
	{
		this->transform.rotation.x = newRotation.x;
		this->transform.rotation.y = newRotation.y;
		this->transform.rotation.z = newRotation.z;

		//#ifdef DEBUG_OUTPUT_CAMERA
		//	printf("CAMERA: forward = %f,%f,%f !\n",transform.forward.x,transform.forward.y,transform.forward.z);
		//	printf("CAMERA: right = %f,%f,%f !\n",transform.right.x,transform.right.y,transform.right.z);
		//	printf("CAMERA: up = %f,%f,%f !\n",transform.up.x,transform.up.y,transform.up.z);
		//#endif

		TransformDIRTY=true;
	}
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
		if(_mode != mode)
			printf("CAMERA: Set to %s-Mode !\n",
					mode==FIRSTPERSON?"FirstPerson":
					mode==FOLLOWTARGET?"FollowTarget":	NULL);

		_mode = mode;
		UpdateView();
	}
	return _mode;
}

BOOL
Cam::ModeAttached(BOOL option)
{
	
	if((NumberOfCameraModes>0)&&(NumberOfCameraModes>option))
	{
		if(option<0)
		{
			return CurrentCamMode+1;
		}
		else if(option==false)
		{
			ModeSocket = NULL;
		}
		else if(option>0)
		{
			return (CurrentCamMode+1)==option;
		}
		return (ModeSocket!=NULL);
	}
}

void // UpdateView: its called on GL-Reshape (if window has been resized...)
Cam::UpdateView(void)
{
	glViewport(0, 0, INPUT->GetViewportRectangle().w,INPUT->GetViewportRectangle().z);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FieldOfView(), Aspect(), 0.1f, 100.0f);
	TransformDIRTY = true;
}

void//recalculates the direction-vectors...
Cam::UpdateDirections(void)
{
	if(TransformDIRTY)
	{
	//	transform.forward = transform.position.direction(transform.rotation);
		transform.forward = (transform.rotation - transform.position).normal();
//		transform.right =  - transform.forward;
	//	Utility::Rotate90(1,transform.right.z,transform.right.x);
	//	Utility::Rotate90(1,transform.up.z,transform.up.y);
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
	if(ModeAttached())
	{
		ModeSocket->UpdateAllActiveModes();
	}

	//update camera position:
	UpdateDirections();
	
	//Set To GL
	gluLookAt(transform.position.x, transform.position.y, transform.position.z,
		transform.rotation.x,transform.rotation.y,transform.rotation.z,	0, 1, 0);

	//update The Position of the attached AudioReciever
	SetAudioResieverPosition(&transform);
	TransformDIRTY = false;
	



	

}

/* * * * User Input * * * */

void
Cam::WheelVRoll(WHEEL state)
{
	ModeSocket->GetCameraMode<Spectator>(_spectator)->mouseWheel(0,state);
}
//
//void
//Cam::keyPress(char key)
//{
//	switch(key)
//	{
//		case 119: // W
//			x += lx * moveSpeed;
//			z += lz * moveSpeed;
//			break;
//		case 115: // S
//			x -= lx * moveSpeed;
//			z -= lz * moveSpeed;
//			break;
//		case 97: // A
//			x += lz * moveSpeed;
//			z += lx * (moveSpeed*-1);
//			break;
//		case 100: // D
//			x -= lz * moveSpeed;
//			z -= lx * (moveSpeed*-1);
//			break;
//	}
//
//
//	//scaleing the camtarget if grabbet..
//	if(key=='g')
//		_target->scale(_target->getTransform()->scale*1.1f);
//	
//	if(key=='k')
//		_target->scale(_target->getTransform()->scale*0.9f);
//
//
//	// Update Transform
//	UpdateTransform();
//}
//
//void
//Cam::specialKeyPressed(int key) 
//{
//	switch (key) {
//		case GLUT_KEY_UP:
//			x += lx * moveSpeed;
//			z += lz * moveSpeed;
//			break;
//		case GLUT_KEY_DOWN:
//			x -= lx * moveSpeed;
//			z -= lz * moveSpeed;
//			break;
//		case GLUT_KEY_LEFT:
//			x += lz * moveSpeed;
//			z += lx * (moveSpeed*-1);
//			break;
//		case GLUT_KEY_RIGHT:
//			x -= lz * moveSpeed;
//			z -= lx * (moveSpeed*-1);
//			break;
//	}
//	
//	// Update Transform
//	UpdateTransform();
//}
//
//void
//Cam::mouseMotion(int newX, int newY)
//{
// 	if(mouseX == 0 && mouseX == 0)
//	{
//		mouseX = newX;
//		mouseY = newY;
//	}
//	
//	int diffX = newX - mouseX;
//	int diffY = newY - mouseY;
//
//	angle += 0.005f * diffX * mouseSpeed;
//	lx = sin(angle);
//	lz = -cos(angle);
//	eyeY += (float)diffY / 300;
//	
//	mouseX = newX;
//	mouseY = newY;
//
//	
//
//	// Update Transform
//	UpdateTransform();
//}

