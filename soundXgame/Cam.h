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
			if(m->IsActive || m->IsDirty)
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

class Cam : public IWheelee, public IConnectable, public IAudioReciever
{
private:
	Vector3					*_targetPosition;		//position vector the camera looks at if in FOLLOWTARGET-Mode...
	IGObject*				_targetObject;
	ConID					*targetConID;			
	double					_fieldOfView;
	GLfloat					_aspect;
	CAM_MODE				_mode;
	float					_distanceToTarget;
	void					UpdateView();			// Update Window and or Viewport Changes...
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
	void		            SetTarget(Vector3* target);
	IGObject*               SetTarget(IGObject* target);
	IGObject*				GetTarget(void);
	Vector3					GetTargetPosition(void);
	float					GetTargetDistance(void);
	void					followTarget(void);
	void					stopFollowing(void);
	virtual void			WheelVRoll(WHEEL state);
	Vector3					move(float x,float y,float z);
	Vector3		   		    move(glm::vec3);
	Vector3					rotate(float x,float y,float z);
	Vector3			        rotate(glm::vec3);
	bool				    ShareAudio(BOOL=3);
	virtual void			Update(void);
	int						NumberOfCameraModes; 
	CameraMode*			    ModeSocket;					// reference to attached mode-extensions...
	BOOL					ModeAttached(BOOL=-1);		// get(): true if any mode-extensions plugt in. set(false): eject's the active mode-extension..
	int						CurrentCamMode;

};

#endif