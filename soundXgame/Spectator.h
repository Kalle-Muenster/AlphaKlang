#ifndef	_CAMERAMODE_SPECTATOR_
#define	_CAMERAMODE_SPECTATOR_

#include "Cam.h"
//#include "CameraMode.h"

class Spectator : public CameraMode , public IInteractive
{

protected:

	int						mouseX, mouseY;			// last-frame mouse position within screen
	float					angle;					// angle of rotation for the camera direction
	float					lx, lz;					// actual vector representing the camera's direction
	float					x, z;					// XZ position of the camera
	float					eyeY;					// head rotation front/back
	float					moveSpeed;				// firstPerson Keyboard moving sensitivity
	float					mouseSpeed;				// firstPerson Mouse sensitivity
	virtual void			UpdateMode(void);		// Updates all datachanges in this Cam-Mode...

public:

	Spectator(void);
	virtual ~Spectator(void);
	static int StaticCamModeID;
	virtual bool Initialize(void);

	virtual void keyPress(char key);
	virtual void specialKeyPressed(int key);
	virtual void mouseMotion(int newX, int newY);
	virtual void mouseWheel(int wheel,WHEEL state);
};

#endif