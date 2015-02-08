#ifndef _CAMERA_MODE__POINTANDCLICK__
#define _CAMERA_MODE__POINTANDCLICK__

#include "Cam.h"
#include "IEditable.h"

class PointAndClick :
	public CameraMode,
	public IInteractive
{
protected:
	GuiObject* guiObject;
	IObjection<IConXtor>* target;
	std::vector<BINDING> bindings;
	std::vector<BINDING>*   targetBindings;
	virtual void UpdateMode(void);
	int lastCamMode;

public:
	static int ID;
	PointAndClick(void);
	virtual ~PointAndClick(void);
	virtual bool Initialize(void);

	virtual void OnActivate(void);
	virtual void keyPress(char key);
	virtual void specialKeyPressed(int key);
	virtual void mouseMotion(int newX, int newY);
	virtual void mouseWheel(int wheel,WHEEL state){}
	void switchBack(void);
	void BindGuiObject(GuiObject*);
};

#endif