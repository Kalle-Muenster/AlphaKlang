#ifndef __CAMERA__
#define __CAMERA__

#include "Utility.h"
#include "InputManager.h"
#include "IGobject.h"
#include "Connectable.h"
#include "AudioObject.h"

enum CAM_MODE : int
{
	ORTHOGRAFIC = 0x1,
	PERSPECTIVE = 0x2,
	FIRSTPERSON = 0x4,
	FOLLOWTARGET = 0x8,
	CAM_MODE_NULL=NULL
};

class Cam : public IInteractive, public IConnectable, public IAudioReciever
{

private:
	IGobject*			_target;
	bool				_isFollowingTarget;
	bool				_FollowFirstPerson;
	float				_distanceToTarget;
	glm::vec3			*camTarget;
	ConID				*targetConID;
	double				_fieldOfView;
	GLfloat				_aspect;
	void                UpdateView();
	CAM_MODE            _mode;
	
	float				angle;			// angle of rotation for the camera direction
	float				lx, lz;			// actual vector representing the camera's direction
	float				x, z;			// XZ position of the camera
	float				eyeY;			// head rotation front/back
	float				moveSpeed;		// firstPerson Cam moving speed

public:
							Cam(void);
	virtual					~Cam(void);
	virtual double			FieldOfView(double = _FNan._Double);
	CAM_MODE                Mode(CAM_MODE=CAM_MODE_NULL);
	virtual GLfloat			Aspect(GLfloat = NULL);
	Transform               transform;
	void					SetTarget(IGobject*);
	void					SetFirstPerson(IGobject*);
	ConID*					SetFirstPerson(IConnectable*);
	IGobject*				GetTarget(void);
	const glm::vec3			GetTargetPosition(void);
	void					followTarget(void);
	void					StopFollowing(void);
	void					Update(void);
	void					SetTargetasFirstPerson(void);
	void					WheelVRoll(WHEEL state);
	BASS_3DVECTOR           move(glm::vec3);
	BASS_3DVECTOR           rotate(glm::vec3);
	bool				    ShareAudio(BOOL=3);

	virtual void			notifyKey(unsigned char key);
	virtual void			specialKeyPressed(int key);
	virtual void			mouseMotion(int newX, int newY);

protected:
	virtual bool IsShared(bool=NULL);
	
	
};

#endif