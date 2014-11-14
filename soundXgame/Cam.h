#ifndef __CAMERA__
#define __CAMERA__

#include "Utility.h"
#include "InputManager.h"
#include "IGObject.h"
#include "Connectable.h"
#include "AudioObject.h"

enum CAM_MODE : int
{
	FIRSTPERSON = 1,
	FOLLOWTARGET = 2,
	TARGETGRABBER = 3,
	CAM_MODE_NULL=NULL
};



class CameraMode : public IConnectable
{
protected:
	virtual void UpdateMode(void){}
	
public:
	Cam* camera;
	bool IsActive;
	bool IsDirty;
	void UpdateAllActiveModes(void)
	{
		CameraMode* m;
		for(int i = 0;i<NumberOfConnectedObjects;i++)
		{
			m = (CameraMode*)getConnectables(ConIDs[i]-1);
			if(m->IsActive && m->IsDirty)
			{	
				m->UpdateMode();
				m->IsDirty=false;
			}
		}
	}
	
	char* ModeName;
	int CamModeID(void)
		{return (int)this->ConnectionID;}

	virtual void SetConnection(IConnectable* camconnector)
		{camera=((CameraMode*)camconnector)->camera;}

	template<typename T> T* AddCameraMode(void)
	{

	Not_hasInitialized();
		
	for(int i=0;i<MAXIMUM_NUMBER_OF_CONNECTIONS;i++)
		if(ConIDs[i] == 0)
		{
			T::StaticCamModeID = i;
			CameraMode* newcon = new T();
			newcon->SetConnection(this);
			newcon->ConnectionID = i+1;
			newcon->ModeName = (char*)typeid(T).name();
			setConnectables(i,(T*)newcon);
			ConIDs[i]=i+1;
			NumberOfConnectedObjects=ConIDs[i];
			return (T*)getConnectables(i);
		}
		return NULL;
	}

	template<typename T> T* GetCameraMode(ConID id)
	{
		return (T*)getConnectables(id-1);
	}

	template<typename T> T* GetCameraModeByStaticID(void)
	{
		return (T*)getConnectables(T::StaticCamModeID);
	}

};


class Cam : public IInteractive, public IConnectable, public IAudioReciever
{
private:
	IGObject*				_target;				//the object the camaera alwas "look's" at if in FOLLOWTARGET-Mode...
	float					_distanceToTarget;		//hold's the actual distance to the cam's target if in FOLLOWTARGET-Mode...
	Vector3					*camTarget;				//position vector the camera looks at if in FOLLOWTARGET-Mode...
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
	IGObject*				SetTarget(IGObject*);
	void					SetTargetAsFirstPerson(IGObject*);
	IGObject*				GrabTarget(void);
	IGObject*				GetTarget(void);
	Vector3					GetTargetPosition(void);
	float					GetTargetDistance(void);
	void					followTarget(void);
	void					StopFollowing(void);
	void					SetTargetasFirstPerson(void);
	virtual void			WheelVRoll(WHEEL state);
	Vector3					move(float x,float y,float z);
	Vector3		   		    move(glm::vec3);
	Vector3					rotate(float x,float y,float z);
	Vector3			        rotate(glm::vec3);
	bool				    ShareAudio(BOOL=3);
	virtual void			Update(void);
	virtual void			keyPress(char key);
	virtual void			specialKeyPressed(int key);
	virtual void			mouseMotion(int newX, int newY);


	int						NumberOfCameraModes; 
	CameraMode*			    ModeSocket;				// reference to attached mode-extensions...
	BOOL					ModeAttached(BOOL=-1);  // get(): true if any mode-extensions plugt in. set(false): eject's the active mode-extension..
	int						CurrentCamMode;

};

#endif