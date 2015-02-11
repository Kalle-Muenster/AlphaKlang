#ifndef _CAMERA_MODE__POINTANDCLICK__
#define _CAMERA_MODE__POINTANDCLICK__

#include "Cam.h"
#include "IEditable.h"

class ObjectMenu : public GuiObject
{
protected:
	IObjection<IConXtor>* attachedObjection;
public:
	ObjectMenu(bool=true);
	ObjectMenu(const char*,bool=true);
	virtual ~ObjectMenu(void);
	void BindToObject(IObjection<IConXtor>* obj);
	bool isAttachedTo(void* obj);
};

class PointAndClick :
	public CameraMode,
	public IInteractive
{
protected:
	ObjectMenu* guiObject;
	IObjection<IConXtor>* target;
	virtual void UpdateMode(void);
	int lastCamMode;

public:
	static int ID;
	PointAndClick(void);
	virtual ~PointAndClick(void);
	virtual bool Initialize(void);
	virtual void OnActivate(void);
	virtual int OnDeactivate(void);
	virtual void keyPress(char key);
	virtual void specialKeyPressed(int key);
	virtual void mouseMotion(int newX, int newY);
	virtual void mouseWheel(int wheel,WHEEL state);
	void switchBack(void);
	void BindGuiObject(GuiObject*);
	void GenerateGUIPanel(void);
};

#endif