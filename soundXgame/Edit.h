#ifndef _CAM_MODE_EDIT_
#define _CAM_MODE_EDIT_

#include "Cam.h"

class Edit :
	public CameraMode , public IInteractive
{

protected:
	
	int						mouseX, mouseY;			// last-frame mouse position within screen
	float					angle;					// angle of rotation for the camera direction
	float					lx, lz;					// actual vector representing the camera's direction
	float					eyeY;					// head rotation front/back
	float					moveSpeed;				// firstPerson Keyboard moving sensitivity
	float					mouseSpeed;				// firstPerson Mouse sensitivity
	virtual void			UpdateMode(void);		// Updates all data-changes in this Cam-Mode...
	bool					accelerate;				// State if currently running faster is enabled, e.g. Shift pressed

public:

	float					x, y, z;					// XZ position of the camera
	static int ID;
	Edit(void);
	virtual ~Edit(void);
	virtual bool Initialize(void);

	virtual void OnActivate(void);
	virtual void keyPress(char key);
//	virtual void specialKeyPressed(int key);
	virtual void mouseMotion(int newX, int newY);
	virtual void mouseWheel(int wheel,WHEEL state){};
	void switchBack(void);
	void BindGuiObject(GuiObject* menu);
};

#endif