#include "Cam.h"
#include "projectMacros.h"

static bool _shareAudioReciever = true;

Cam::Cam(void) : angle(0),lx(0),lz(-1),x(0),z(5),eyeY(1),moveSpeed(0.1f),mouseSpeed(1.0f),mouseX(0),mouseY(0)
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
	_transformChanged=true;

	_fieldOfView = 55;
	_aspect = 16.0/9.0;
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(transform.position.x, transform.position.y, transform.position.z, transform.rotation.x,transform.rotation.y,transform.rotation.z, 0, 1, 0);
	_mode = CAM_MODE::FIRSTPERSON;

	InputManager* Input = InputManager::getInstance();
	Input->attachKey(this);
	Input->attachMouseMove(this);
	Input->attachSpecial(this);

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
	_FollowFirstPerson=false;
	delete camTarget;
}

void
Cam::SetTarget(IGobject *targetObject)
{
	_distanceToTarget = 0.1;
	this->camTarget = &targetObject->getTransform()->position;
	_target = targetObject;
	_target->IsVisible=true;
	//_FollowFirstPerson=false;
	this->Mode(CAM_MODE::FOLLOWTARGET);
	printf("CAMERA: Set %s-ID:%i As Follow-Target!\n",_target->GetName(),_target->GetID());
}

IGobject* 
Cam::GetTarget(void)
{
	if(_isFollowingTarget||_FollowFirstPerson)
		return _target;
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

/* * * * Transform * * * */

void
Cam::UpdateTransform(void)
{
	this->move(x, eyeY, z);
	this->rotate(x+lx, 1.0f,  z+lz);
	this->_transformChanged = true;
}

BASS_3DVECTOR
Cam::move(float x,float y,float z)
{
	return move(Vector3(x,y,z));
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

	SetAudioResieverPosition(&this->transform);

	//if(transform.movement.x!=0 || transform.movement.y!=0 || transform.movement.z!=0)
		//printf("\nCAMERA: moved to: %f,%f,%f !",transform.position.x,transform.position.y,transform.position.z);
	
	return transform.position;
}

BASS_3DVECTOR
Cam::rotate(float x,float y,float z)
{
	return rotate(Vector3(x,y,z));
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
		printf("\nCAMERA: Set to %s-Mode !",_mode==FIRSTPERSON?"FirstPerson":_mode==PERSPECTIVE?"Perspective":_mode==FOLLOWTARGET?"FollowTarget":NULL);
	}
	return _mode;
}

void // UpdateView: its called on GL-Reshape (if window has been resized...)
Cam::UpdateView(void)
{
	glViewport(0, 0, INPUT->GetViewportRectangle().w,INPUT->GetViewportRectangle().z);
	
	switch(Mode())
	{
		case PERSPECTIVE:
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			gluPerspective(FieldOfView(), Aspect(), 0.1f, 100.0f);
			break;
	/*	case ORTHOGRAFIC:
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			gluOrtho2D(0,SCREENWIDTH,SCREENHEIGHT,0);
			gluLookAt(0,0,0,0,0,0,0,1,0);
			break;*/
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
		this->_transformChanged = true;
	}

	if(Mode()==FIRSTPERSON)
	{
		this->UpdateTransform();
		//this->move(x, eyeY, z);
		//this->rotate(Vector3(x+lx, 1.0f,  z+lz));
		// TODO update just if values changed.. does not work so gar
	}

	// if transform changed -> update camera
	if(this->_transformChanged == true)
	{
		gluLookAt(transform.position.x, transform.position.y, transform.position.z,
			transform.rotation.x,transform.rotation.y,transform.rotation.z,
			0, 1, 0);
		this->_transformChanged = false;
	}


	//update The Position of the attached AudioReciever
	SetAudioResieverPosition(&transform);
}

/* * * * User Input * * * */

void
Cam::WheelVRoll(WHEEL state)
{
	transform.position.y-=(float)state/10;
	FieldOfView(FieldOfView() - state); 
	//printf("\nCAMERA: moved to: %f,%f,%f !\n",transform.position.x,transform.position.y,transform.position.z);
}

void
Cam::keyPress(unsigned char key)
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