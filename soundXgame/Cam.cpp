#include "Cam.h"
#include "projectMacros.h"

#define DEBUG_OUTPUT_CAMERA

static bool _shareAudioReciever = true;
bool _targetGRABBED=false;


Cam::Cam(void) :
	angle(0),
	lx(0),
	lz(-1),
	x(0),z(5),
	eyeY(1),
	moveSpeed(0.1f),
	mouseSpeed(1.0f),
	mouseX(0),
	mouseY(0)
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
	_transformChanged=true;

	_fieldOfView = 55;
	_aspect = 16.0/9.0;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(transform.position.x, transform.position.y, transform.position.z, transform.rotation.x,transform.rotation.y,transform.rotation.z, 0, 1, 0);
	//_mode = CAM_MODE::PERSPECTIVE;

	
	INPUT->attachKey(this);
	INPUT->attachMouseMove(this);
	INPUT->attachSpecial(this);
	
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
	delete camTarget;
}

IGobject*
Cam::SetTarget(IGobject *targetObject)
{
	_distanceToTarget = transform.position.distance(targetObject->getTransform()->position);
	this->camTarget = &targetObject->getTransform()->position;
	_target = targetObject;
	_target->IsVisible=true;
	printf("CAMERA: Set %s-ID:%i As Follow-Target!\n",_target->GetName(),_target->GetID());
	return targetObject;
}

IGobject* 
Cam::GetTarget(void)
{
	return _target;
}

void
Cam::followTarget()
{
	if (this->camTarget)
	{
		Mode(FOLLOWTARGET);
		_target->IsVisible=true;
	}
}

void
Cam::StopFollowing()
{
	Mode();
	this->GetTarget()->IsVisible=true;
//	_target->IsVisible=true;
}

void
Cam::SetTargetAsFirstPerson(IGobject* personObj)
{
	_target = personObj;
	this->camTarget = &personObj->getTransform()->position;
	_target->IsVisible=false;
	Mode(FIRSTPERSON_CONTROLLER);
}



IGobject*
Cam::GrabTarget(void)
{
	if(!_target)
		return NULL;
	_targetGRABBED = true;
		
}

float
Cam::GetTargetDistance(void)
{
	if(_target)
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
	if(_target)
		return *this->camTarget;
	else
		return transform.position;
}

/* * * * Transform * * * */

void
Cam::UpdateTransform(void)
{
	this->move(x, eyeY, z);
	this->rotate(x+lx, 1.0f, z+lz);
	this->_transformChanged = TransformDIRTY;
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

		#ifdef DEBUG_OUTPUT_CAMERA
			printf("CAMERA: moved to: %f,%f,%f !\n",transform.position.x,transform.position.y,transform.position.z);
		#endif
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

		#ifdef DEBUG_OUTPUT_CAMERA
			printf("CAMERA: forward = %f,%f,%f !\n",transform.forward.x,transform.forward.y,transform.forward.z);
			printf("CAMERA: right = %f,%f,%f !\n",transform.right.x,transform.right.y,transform.right.z);
			printf("CAMERA: up = %f,%f,%f !\n",transform.up.x,transform.up.y,transform.up.z);
		#endif

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
		transform.forward = transform.right = transform.up = transform.position.direction(transform.rotation);
		Utility::Rotate90(1,transform.right.z,transform.right.x);
		Utility::Rotate90(1,transform.up.z,transform.up.y);


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

//	if(_isFollowingTarget)
	if(Mode()==FOLLOWTARGET)
	{
		this->move(transform.position);
		this->rotate(*camTarget);
	}

	if(Mode()==FIRSTPERSON)
	{
		this->UpdateTransform();
	}



	//update camera position:
	UpdateDirections();
	
	gluLookAt(transform.position.x, transform.position.y, transform.position.z,
		transform.rotation.x,transform.rotation.y,transform.rotation.z,	0, 1, 0);

	//update The Position of the attached AudioReciever
	SetAudioResieverPosition(&transform);

	
	if(_targetGRABBED)
	{
		_targetGRABBED = !INPUT->Mouse.LEFT.DOUBLE;
		_target->getTransform()->position = (this->transform.position + (this->transform.forward * GetTargetDistance())) + (INPUT->Mouse.MIDDLE.HOLD? (this->transform.forward * -INPUT->Mouse.Movement.y/10) : Vector3(0,0,0));
	}
	else if(INPUT->Mouse.LEFT.DOUBLE)
		GrabTarget();


	TransformDIRTY = false;

}

/* * * * User Input * * * */

void
Cam::WheelVRoll(WHEEL state)
{
	transform.position.y-=(float)state/10;
	FieldOfView(FieldOfView() - state); 
#ifdef DEBUG_OUTPUT_CAMERA
	printf("CAMERA: moved to: %f,%f,%f !\n",transform.position.x,transform.position.y,transform.position.z);
#endif
}

void
Cam::keyPress(char key)
{
	switch(key)
	{
		case 119: // W
			x += lx * moveSpeed;
			z += lz * moveSpeed;
			break;
		case 115: // S
			x -= lx * moveSpeed;
			z -= lz * moveSpeed;
			break;
		case 97: // A
			x += lz * moveSpeed;
			z += lx * (moveSpeed*-1);
			break;
		case 100: // D
			x -= lz * moveSpeed;
			z -= lx * (moveSpeed*-1);
			break;
	}
	// Update Transform
	UpdateTransform();
}

void
Cam::specialKeyPressed(int key) 
{
	switch (key) {
		case GLUT_KEY_UP:
			x += lx * moveSpeed;
			z += lz * moveSpeed;
			break;
		case GLUT_KEY_DOWN:
			x -= lx * moveSpeed;
			z -= lz * moveSpeed;
			break;
		case GLUT_KEY_LEFT:
			x += lz * moveSpeed;
			z += lx * (moveSpeed*-1);
			break;
		case GLUT_KEY_RIGHT:
			x -= lz * moveSpeed;
			z -= lx * (moveSpeed*-1);
			break;
	}
	
	// Update Transform
	UpdateTransform();
}

void
Cam::mouseMotion(int newX, int newY)
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

	

	// Update Transform
	UpdateTransform();
}