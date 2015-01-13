/*InputManager*/
#ifndef __INPUTMANAGER_H__
#define __INPUTMANAGER_H__

//Undefine to switch off Console-output of mouse data test or FPS calculation
#define MOUSE_TEST_OUTPUT (1)
//#define CONSOLE_OUTPUT_FPS

//Observer Flags
#define OBSERVATE_CLICKS       (0x1)
#define OBSERVATE_WHEELS       (0x2)
#define OBSERVATE_KEYBOARD     (0x4)
#define OBSERVATE_MOUSE        (0x8)

//Maximum amount on Observers an Event can handle...
#define MAXIMUM_NUMBER_ON_OBSERVERS_PER_EVENT (1000)

#include <vector>
#include "projectGrafics.h"
//#include "projectMacros.h"
#include "Utility.h"

enum WHEEL{DOWN=-1,NONE=0,UP=1};

struct MouseButton
{
public:
	bool CLICK,HOLD,DOUBLE,RELEASE;
};


//IObserver Interfaces
class IObserver
{
public:
	virtual int observedEvents()=0;
	
public:
	virtual bool checkForObservability(int flag);
};

class IInteractive : public IObserver
{
protected:
	int observedEvents(); 
public:
	IInteractive(void);
	virtual void specialKeyPressed(int key){};
	virtual void keyPress(char key){};
	virtual void mouseMotion(int x, int y){};
	virtual void mouseClicks(int button,bool IsPressed,VectorF position){};
	virtual void mouseWheel(int wheel,WHEEL state){};
};

class IClickable : public IObserver
{
protected:
	int observedEvents(); 
public:
	IClickable(void);
	virtual void LeftClick(VectorF){};
	virtual void RightClick(VectorF){};
	virtual void MiddleClick(VectorF){};
	virtual void LeftRelease(VectorF){};
	virtual void RightRelease(VectorF){};
	virtual void MiddleRelease(VectorF){};
};

class IWheelee : public IObserver
{
protected:
	int observedEvents();  
public:
	IWheelee(void);
	virtual void WheelVRoll(WHEEL state){};
	virtual void WheelHRoll(WHEEL state){};
};

#include "List.h"
#include "UpdateManager.h"

class InputManager
//	: public IUpdateble
{
	//Con-and Destruction....
private:
	InputManager(void);

public:
	static const char* start(void);
	static InputManager* getInstance();
	virtual ~InputManager(void);


	//Private Stuff...........................
private:
	//Invokation-Lists:
	List<unsigned char, 30> keyList;
	List<IObserver*,MAXIMUM_NUMBER_ON_OBSERVERS_PER_EVENT> MouseMotionObservers;
	List<IObserver*,MAXIMUM_NUMBER_ON_OBSERVERS_PER_EVENT> KeyboardObservers;
	List<IObserver*,MAXIMUM_NUMBER_ON_OBSERVERS_PER_EVENT> SpecialKeyObservers;
	List<IObserver*,MAXIMUM_NUMBER_ON_OBSERVERS_PER_EVENT> MouseWheelObservers;
	List<IObserver*,MAXIMUM_NUMBER_ON_OBSERVERS_PER_EVENT> mouseClickListener;
	

	/*private vars and functions
	 (used for update inputs)*/
	bool LEFTnewState,MIDDLEnewState,RIGHTnewState;
	void setMouseButtons(void);
	void setMousePosition(int,int);
	void notifyWheel(int wheel);
	void notifyQlicks(void);
	void notifyJoystick(int id,int button,bool state,int aX,int aY,int aZ);
	void notifyKey();
	int calculateFPS(void);

	//Public Stuff...........................
public:
	// Mouse: hold's actual Mouse-StateData
 struct {
	WHEEL WheelV;
	WHEEL WheelH;
	MouseButton LEFT;
	MouseButton RIGHT;
	MouseButton MIDDLE;
	VectorF Movement; //deltaMove
	VectorF Position;
	int X,Y;
}   Mouse; 
 struct { //GameControler 1:
	bool Enabled;
	unsigned short NumberOfAxis;
	unsigned short NumberOfButtons;
	unsigned Buttons;
	float aX,aY,aR,aZ;
 }	Controler1;
 struct { //GameControler 2:
	bool Enabled;
	unsigned short NumberOfAxis;
	unsigned short NumberOfButtons;
	unsigned Buttons;
	float aX,aY,aR,aZ;
 }	Controler2;
	ProjectMappe::Rectangle* GetViewportRectangle(void);
	void SetDoubleclickTime(int milliseconds);
	double FrameTime; //FrameTime like deltaTime -> hold's actual timeDiff since last Frame in seconds

    /*attachement functions by wich "IObserver"-objects
    can register on Notification-Events */
	void attachMouseMove(IObserver* obs);
	void attachKey(IObserver* obs);
	void attachSpecial(IObserver* obs);
	void attachMouseClick(IObserver* obs);
	void attachMouseWheel(IObserver* obs);

	/*detach-functions to sign out from lists...*/
	void DetachMouseMove(IObserver* obs);
	void DetachKeyboard(IObserver* obs);
	void DetachSpecialkeys(IObserver* obs);
	void DetachMouseWheel(IObserver* obs);

	//Update-functions called by main-Api...
	void notifySpecialKey(int key);
	void registerKey(unsigned char key);
	void registerKeyUp(unsigned char key);
	void notifyMouse(int x, int y);
	void UpdateMouseButtons(int,int,int,int);
	void UpdateMouseWheel(int,int,int,int);
	void SaveViewportRectangle(int x,int y,int w,int h);
	void UpdateJoysticks(int id, unsigned buttons,int AxisX,int AxisY,int AxisZ);

	void FireEvents(void);

	//frameReset: must be called before all other
	//i.E. as first function in the UpdateFunction 
	//or as last function just below swapBuffer()
	void PerFrameReset(void);

};

#endif