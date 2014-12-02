#ifndef _TARGETGRABBER_H_
#define _TARGETGRABBER_H_

#include "UpdateManager.h"
#include "UpdateManager.h"
#include "Connectable.h"





/////////////////////////////////////////////////////
/* CameraTargetGrabber: 
 * the targetgrabber is something like a editing tool.
 * it provides Translation,Rotation,and Scaling of 
 * of 3D-Objects in the Scene...
 */ 
#include "Cam.h"
class TargetGrabber :
	public CameraMode
{

public:
	enum class MODE : short
	{get= -1,OFF=0,MOVE=1,ROTATE=2,SCALE=3, };
	TargetGrabber::MODE Mode(TargetGrabber::MODE = TargetGrabber::MODE::get);
	TargetGrabber(void);
	virtual ~TargetGrabber(void);
	static int StaticCamModeID;
	virtual bool Initialize(void);
	IGObject* GrabTarget(void);
	bool IsATargetGrabbed(void);
	void ReleaseTarget(void);

	
protected:
	virtual void UpdateMode(void);
	bool _targetGRABBED;
	ConID targetConnectedMode[3];
	int targetSwitcher;
};

//////////////////////////////////////////
/*This are tree helper classes for the 
 *TargetGrabber-Camera-Mode. the TargetGrabber
 *will connect these "connectables" to the 
 *grabbed Object that will be edited. */ 
class CamTargetRotator : 
	public IConnectable,
	public IUpdateble
{
private:
	void RotateOnMouseHold(void);
public:
	CamTargetRotator(void);
	virtual ~CamTargetRotator(void);
	virtual void DoUpdate(void);
};
//////////////////////////////////////////
class CamTargetMover : 
	public IConnectable,
	public IInteractive
{
public:
    CamTargetMover(void);
    virtual ~CamTargetMover(void);
    virtual bool Initialize(void);
    bool IsActive;
    virtual void mouseMotion(int x,int y);
};
//////////////////////////////////////////
class CamTargetScaler :
	public IConnectable,
	public IUpdateble
{
private:
	Vector3 vec;
public:
	CamTargetScaler(void);
	virtual bool Initialize(void);
	virtual ~CamTargetScaler(void);
	virtual void DoUpdate(void);
};
//////////////////////////////////////////

#endif