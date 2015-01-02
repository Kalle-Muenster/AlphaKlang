#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Utility.h"
#include "InputManager.h"
#include "IGObject.h"
#include "Connectable.h"
#include "AudioObject.h"


enum CAM_MODE : int
{
	set = -1,
	FOLLOWTARGET = 0,
	FIRSTPERSON = 1,
	SPECTATOR = 2,
	TARGETGRABBER = 3
};



class CameraMode : public IConnectable
{
protected:
	virtual void UpdateMode(void){}
	bool isPrimarMode;
public:
	Cam* camera;
	char* ModeName;
	bool IsActive;
	bool IsDirty;
	bool IsPrimarMode(void)
	{return isPrimarMode;}
	virtual ~CameraMode(void)
		{}
	
	void UpdateAllActiveModes(void)
	{
		if(NumberOfConnectedObjects<=0)
			return;

		CameraMode* m;
		for(int i = 0;i<NumberOfConnectedObjects;i++)
		{
			m = (CameraMode*)getConnectables(ConIDs[i]);
			if(m->IsActive || m->IsDirty)
			{	
				m->UpdateMode();
				m->IsDirty=false;
			}
		}
	}
	
	int CamModeID(void)
		{return (int)this->ConnectionID;}

	virtual void SetConnection(IConnectable* camconnector)
		{camera=((CameraMode*)camconnector)->camera;}

	template<typename T> T* AddCameraMode(void)
	{

	Not_hasInitialized();
		
	for(int i=0;i<MAXIMUM_NUMBER_OF_CONNECTIONS;i++)
		if(ConIDs[i] == EMPTY_SLOT)
		{
			T::StaticCamModeID = i;
			CameraMode* newcon = new T();
			newcon->SetConnection(this);
			newcon->ConnectionID = ConIDs[i] = i;
			newcon->ModeName = (char*)typeid(T).name();
			setConnectables(i,(T*)newcon);
			NumberOfConnectedObjects++;
			this->camera->NumberOfModes++;
			return (T*)getConnectables(i);
		}
		return NULL;
	}

	template<typename T> T* Get(ConID id)
	{
		return (T*)getConnectables(id);
	}

	template<typename T> T* GetCameraMode(void)
	{
		return (T*)getConnectables(T::StaticCamModeID);
	}

};

class Cam : public IWheelee,  public IAudioReciever
{
private:
	static bool				_shareAudioReciever;
	Vector3					*_targetPosition;		//position vector the camera looks at if in FOLLOWTARGET-Mode...
	IGObject*				_targetObject;
	ConID					*targetConID;			
	double					_fieldOfView;
	GLfloat					_aspect;
	int						_mode;
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
	Vector3		   		    move(Vector3);
	Vector3					rotate(float x,float y,float z);
	Vector3			        rotate(Vector3);
	bool				    ShareAudio(BOOL=3);
	virtual void			Update(void);
	int						NumberOfModes; 
	CAM_MODE                Mode(CAM_MODE = set);
	CameraMode*			    ModeSocket;					// reference to attached mode-extensions...
	template<class cM> cM*	mode(CAM_MODE value=set)
	{
		if(value==set)
			return ModeSocket->Get<cM>(Mode(set));
		return ModeSocket->Get<cM>(value);
	}
};

#endif