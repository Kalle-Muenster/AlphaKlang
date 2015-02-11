#ifndef _FIRSTPERSON_CAMERA_
#define _FIRSTPERSON_CAMERA_

#include "Cam.h"
#include "Ground.h"
#include "VoxelVector.h"


class FirstPerson : public CameraMode , public IInteractive
{
protected:
	bool					NeedViewUpdate;
	int						mouseX, mouseY;			// last-frame mouse position within screen
	float					angle;					// angle of rotation for the camera direction
	float					lx, lz;					// actual vector representing the camera's direction
	float					eyeY;					// head rotation front/back
	float					moveSpeed;				// firstPerson Keyboard moving sensitivity
	float					mouseSpeed;				// firstPerson Mouse sensitivity
	virtual void			UpdateMode(void);		// Updates all data-changes in this Cam-Mode...
	bool					accelerate;				// State if currently running faster is enabled, e.g. Shift pressed
	bool					MouseCapture;			// Indicates if the mouse-cursor always will be centered at screen-center.

public:
	float					x, y, z;					// XZ position of the camera
	FirstPerson(void);
//	virtual ~FirstPerson(void);
	static int ID;
	void UpdateHeight(float y);
	virtual void OnActivate(void);
	virtual int OnDeactivate(void);
	virtual bool Initialize(void);
	virtual void keyPress(char key);
	virtual void specialKeyPressed(int key);
	virtual void mouseMotion(int newX, int newY);
	virtual void mouseWheel(int wheel,WHEEL state){};
	VectorF GetPlayerPosition(void);
	void DisableMouseCapture(void);
	void EnableMouseCapture(void);
};

#endif