#ifndef	_CAMERAMODE_SPECTATOR_
#define	_CAMERAMODE_SPECTATOR_

#include "Cubus.h"
#include "Cam.h"
//#include "CameraMode.h"

class Spectator : 
	public CameraMode, 
	public IInteractive
{

protected:
	virtual void UpdateMode(void);		// Updates all datachanges in this Cam-Mode...
	bool cameraWillMove;
	Vector3 vec;
	Vector3 rotary3;
	
	float delta;
public:

	Spectator(void);
	virtual ~Spectator(void);
	static int ID;
	virtual bool Initialize(void);
//	Cubus* CamCubus;
	virtual void keyPress(char key);
	virtual void specialKeyPressed(int key);
//	virtual void mouseMotion(int newX, int newY);
//	virtual void mouseWheel(int wheel,WHEEL state);
};

#endif