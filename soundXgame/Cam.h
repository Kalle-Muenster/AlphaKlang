#ifndef __CAMERA__
#define __CAMERA__

#include "Utility.h"
#include "InputManager.h"
#include "IGobject.h"
#include "Connectable.h"
#include "AudioObject.h"

enum CAM_MODE : int
{
	//ORTHOGRAFIC = 0x1,
	//PERSPECTIVE = 0x2,
	FIRSTPERSON = 0x1,
	FOLLOWTARGET = 0x2,
	FIRSTPERSON_CONTROLLER = 0x4,
	CAM_MODE_NULL=NULL
};

class Cam : public IInteractive, public IConnectable, public IAudioReciever
{



private:
	IGobject*				_target;				//the object the camaera alwas "look's" at if in FOLLOWTARGET-Mode...
	float					_distanceToTarget;		//hold's the actual distance to the cam's target if in FOLLOWTARGET-Mode...
	Vector3					*camTarget;				//pasition vector the camera looks at if in FOLLOWTARGET-Mode...
	ConID					*targetConID;			
	double					_fieldOfView;
	GLfloat					_aspect;
	CAM_MODE				_mode;
	
	int						mouseX, mouseY;			// last-frame mouse position within screen
	float					angle;					// angle of rotation for the camera direction
	float					lx, lz;					// actual vector representing the camera's direction
	float					x, z;					// XZ position of the camera
	float					eyeY;					// head rotation front/back
	float					moveSpeed;				// firstPerson Keyboard moving sensitivity
	float					mouseSpeed;				// firstPerson Mouse sensitivity
	bool					_transformChanged;		// flag if last frame the transform has changed

	void					UpdateView();			// Update Window and or Viewport Changes...
	void					UpdateTransform(void);	// Update move and rotate Transform
	void					UpdateDirections(void); // Re-Calculates "forward","right" and "up"
protected:
	virtual bool			IsShared(bool=NULL);
	bool					TransformDIRTY;

public:
							Cam(void);
	virtual					~Cam(void);
	virtual double			FieldOfView(double = _FNan._Double);
	CAM_MODE                Mode(CAM_MODE = (CAM_MODE)NULL);
	virtual GLfloat			Aspect(GLfloat = NULL);
	Transform               transform;					
	IGobject*				SetTarget(IGobject*);
	void					SetTargetAsFirstPerson(IGobject*);
	IGobject*				GrabTarget(void);
	IGobject*				GetTarget(void);
	Vector3					GetTargetPosition(void);
	float					GetTargetDistance(void);
	void					followTarget(void);
	void					StopFollowing(void);
	void					SetTargetasFirstPerson(void);
	void					WheelVRoll(WHEEL state);
	Vector3					move(float x,float y,float z);
	Vector3		   		    move(glm::vec3);
	Vector3					rotate(float x,float y,float z);
	Vector3			        rotate(glm::vec3);
	bool				    ShareAudio(BOOL=3);
	void					Update(void);
	virtual void			keyPress(char key);
	virtual void			specialKeyPressed(int key);
	virtual void			mouseMotion(int newX, int newY);


	
	
};

#endif