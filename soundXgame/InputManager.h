/*InputManager*/
#ifndef __INPUTMANAGER_H__
#define __INPUTMANAGER_H__

//Undefine to switch off Console-output of mouse data test
#define MOUSE_TEST_OUTPUT (1)

//Observer Flags
#define OBSERVATE_CLICKS       (0x1)
#define OBSERVATE_WHEELS       (0x2)
#define OBSERVATE_KEYBOARD     (0x4)
#define OBSERVATE_MOUSE        (0x8)

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
	virtual void mouseQlicks(int button,bool IsPressed,glm::vec2 position){};
	virtual void mouseWheel(int wheel,WHEEL state){};
};

class IQlickable : public IObserver
{
protected:
	int observedEvents(); 
public:
	IQlickable(void);
	virtual void LeftClick(glm::vec2 position){};
	virtual void RightClick(glm::vec2 position){};
	virtual void MiddleClick(glm::vec2 position){};
	virtual void LeftRelease(glm::vec2 position){};
	virtual void RightRelease(glm::vec2 position){};
	virtual void MiddleRelease(glm::vec2 position){};
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


class InputManager
{
	//Con-and Destruction....
private:
	InputManager(void);
public:
	static InputManager* getInstance();
	virtual ~InputManager(void);


	//Private Stuff...........................
private:
	//Invokation-Lists:
	std::vector<IObserver*> mouseMoveListener;
	std::vector<IObserver*> keyListener;
	std::vector<IObserver*> specialListener;
	std::vector<IObserver*> mouseClickListener;
	std::vector<IObserver*> mouseWheelListener;

	/*private vars and functions
	 (used for update inputs)*/
	bool LEFTnewState,MIDDLEnewState,RIGHTnewState;
	void setMouseButtons(void);
	void setMousePosition(int,int);
	void notifyWheel(int wheel);
	void notifyQlicks(void);
	void notifyJoystick(int id,int button,bool state,int aX,int aY,int aZ);


	//Public Stuff...........................
public:
	// Mouse: hold's actual Mouse-StateData
 struct {
	WHEEL WheelV;
	WHEEL WheelH;
	MouseButton LEFT;
	MouseButton RIGHT;
	MouseButton MIDDLE;
	glm::vec2 Movement; //deltaMove
	glm::vec2 Position;
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
	glm::vec4 GetViewportRectangle(void);
	void SetDoubleclickTime(int milliseconds);
	double FrameTime; //deltaTime...

    /*attachement functions by wich "IObserver"-objects
    can register on Notification-Events */
	virtual void attachMouseMove(IObserver* obs);
	virtual void attachKey(IObserver* obs);
	virtual void attachSpecial(IObserver* obs);
	virtual void attachMouseClick(IObserver* obs);
	virtual void attachMouseWheel(IObserver* obs);


	//Update-functions called by main-Api...
	virtual void notifySpecialKey(int key);
	virtual void notifyKey(char key);
	virtual void notifyMouse(int x, int y);
	void UpdateMouseButtons(int,int,int,int);
	void UpdateMouseWheel(int,int,int,int);
	void SaveViewportRectangle(int x,int y,int w,int h);
	void UpdateJoysticks(int id, unsigned buttons,int AxisX,int AxisY,int AxisZ);


	//frameReset: must be called before all other
	//i.E. as firs function in the UpdateFunction 
	//or as last function just below swapBuffer()
	void PerFrameReset(void);

};

#endif