#ifndef __CAMERA__
#define __CAMERA__

#include "Utility.h"
#include "InputManager.h"
#include "IGObject.h"
#include "Connectable.h"
#include "AudioObject.h"


enum CAM_MODE : int
{
	get = -1,
	FOLLOWTARGET = 1,
	FIRSTPERSON = 2,
	SPECTATOR = 3,
	TARGETGRABBER = 4
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
			m = (CameraMode*)getConnectables(ConIDs[i]-1);
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
		if(ConIDs[i] == 0)
		{
			T::StaticCamModeID = i;
			CameraMode* newcon = new T();
			newcon->SetConnection(this);
			newcon->ConnectionID = ConIDs[i] = i+1;
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
		return (T*)getConnectables(id-1);
	}

	template<typename T> T* GetCameraMode(void)
	{
		return (T*)getConnectables(T::StaticCamModeID-1);
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
	Vector3		   		    move(glm::vec3);
	Vector3					rotate(float x,float y,float z);
	Vector3			        rotate(Vector3);
	bool				    ShareAudio(BOOL=3);
	virtual void			Update(void);
	int						NumberOfModes; 
	CAM_MODE                Mode(CAM_MODE = get);
	CameraMode*			    ModeSocket;					// reference to attached mode-extensions...
	template<typename CMod> CMod* Get(void)
	{
		return ModeSocket->GetCameraMode<CMod>();
	}
};

#endif