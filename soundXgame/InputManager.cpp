#include "InputManager.h"
#include "DataStructs.h"

LPSYSTEMTIME _sTime;
DWORD _timerTicks;
DWORD _lastTicks;
DWORD _frameTicks;
DWORD _clicktimer;
DWORD _doubleClickLength;

InputManager* instance;

glm::vec4 Viewport;
Vector3 ViewPortNormalizedMouseCoordinates;
bool _buttonChange[16];
bool _axisChange[16];

bool _HasFlag(unsigned value,unsigned flag)
{
	return (value / flag > 0);
}

double _TicksToSeconds(DWORD ticks)
{
	return (float)ticks/1000.0;
}

double 
_updateTimer(void)
{
	_frameTicks = GetTickCount() - _lastTicks;
	_clicktimer = ((_clicktimer > 0)&&(_clicktimer<=_doubleClickLength))? (_clicktimer - _frameTicks):0;
	_lastTicks += _frameTicks;
	return _TicksToSeconds(_frameTicks);
}

InputManager* InputManager::getInstance() 
{
	if(!instance) 
	{
		_lastTicks = GetTickCount();
		_frameTicks = 0;
		_doubleClickLength = 200;
		instance = new InputManager();
	}

	return instance;
}

InputManager::InputManager(void)
{
	this->keyListener = std::vector<IObserver*>();
	this->specialListener = std::vector<IObserver*>();
	this->mouseMoveListener = std::vector<IObserver*>();
	this->mouseClickListener = std::vector<IObserver*>();
	this->mouseWheelListener = std::vector<IObserver*>();

	FrameTime = 0;
	Mouse.X=Mouse.Y=0;
	Mouse.Position.x=Mouse.Position.y=0.0f;
	LEFTnewState=MIDDLEnewState=RIGHTnewState=false;
	Mouse.LEFT.CLICK=Mouse.RIGHT.CLICK=Mouse.MIDDLE.CLICK=false;
	Mouse.LEFT.HOLD=Mouse.RIGHT.HOLD=Mouse.MIDDLE.HOLD=false;
	Mouse.LEFT.DOUBLE=Mouse.RIGHT.DOUBLE=Mouse.MIDDLE.DOUBLE=false;
	Mouse.Movement = glm::vec2(0,0);

	Viewport=glm::vec4(0,0,1,1);
	ViewPortNormalizedMouseCoordinates=Vector3(0,0,0);

	Controler1.Enabled=true;
	Controler1.NumberOfButtons=10;
	Controler1.NumberOfAxis=3;
/*
	if(glutGet(GLUT_HAS_JOYSTICK))
	{
		Controler1.NumberOfAxis = glutJoystickGetNumAxes(0);
		if(Controler1.NumberOfAxis>0)
		{
			Controler1.Enabled=true;
			glutJoystickSetCenter(0,0);
			Controler1.aX=1;
			glutJoystickSetMaxRange(0,&Controler1.aX);
			Controler1.aX=-1;
			glutJoystickSetMinRange(0,&Controler1.aX);
			for(int i = 0;i<Controler1.NumberOfAxis;i++)
				glutJoystickSetDeadBand(0,i,0.1);
			Controler1.Buttons = glutJoystickGetNumButtons(0);
		}
	}
*/
}

InputManager::~InputManager(void)
{
	delete _sTime;
}

void InputManager::attachMouseMove(IObserver* obs) 
{
	if(obs->checkForObservability(OBSERVATE_MOUSE))
		this->mouseMoveListener.push_back(obs);
}

void InputManager::attachKey(IObserver* obs) 
{
	if(obs->checkForObservability(OBSERVATE_KEYBOARD))
		this->keyListener.push_back(obs);
}

void InputManager::attachSpecial(IObserver* obs) 
{
	if(obs->checkForObservability(OBSERVATE_KEYBOARD))
		this->specialListener.push_back(obs);
}

void InputManager::attachMouseClick(IObserver* obs)
{
	if(obs->checkForObservability(OBSERVATE_MOUSE)||obs->checkForObservability(OBSERVATE_CLICKS))
		this->mouseClickListener.push_back(obs);
}

void InputManager::attachMouseWheel(IObserver* obs)
{
	if(obs->checkForObservability(OBSERVATE_WHEELS))
		this->mouseWheelListener.push_back(obs);
}



void 
InputManager::notifyJoystick(int id,int button,bool state,int AxisX,int AxisY,int AxisZ)
{
	//Todo:  implement Invokationlist for JoysticObserverCalbacks...
}

void
InputManager::notifySpecialKey(int key) 
{
	for(auto it = this->specialListener.begin(); it != this->specialListener.end(); ++it) 
	{
		((IInteractive*)(*it))->specialKeyPressed(key);
	}
}

void
InputManager::notifyKey(char key) 
{
	for(auto it = this->keyListener.begin(); it != this->keyListener.end(); ++it) 
	{
		((IInteractive*)(*it))->keyPress(key);
	}
}

void
InputManager::notifyMouse(int x, int y) 
{
	setMousePosition(x,y);
	for(auto it = this->mouseMoveListener.begin(); it != this->mouseMoveListener.end(); ++it) 
	{
		((IInteractive*)(*it))->mouseMotion(x,y);
	}
}

void
InputManager::notifyWheel(int wheel)
{
	for(auto it=mouseWheelListener.begin();it!=mouseWheelListener.end();it++)
	{
		if(!(*it)->checkForObservability(OBSERVATE_MOUSE|OBSERVATE_KEYBOARD))
		{
			if(wheel==0)
				((IWheelee*)(*it))->WheelVRoll(Mouse.WheelV);
			else
				((IWheelee*)(*it))->WheelHRoll(Mouse.WheelH);
		}
		else
			((IInteractive*)(*it))->mouseWheel(wheel,wheel==0?Mouse.WheelV:Mouse.WheelH);
	}
}

void
InputManager::notifyQlicks(void)
{
	bool click = false;
	if(Mouse.LEFT.CLICK)
	{click=true;
		for(auto it = this->mouseClickListener.begin(); it != this->mouseClickListener.end(); ++it) 
		{
			if((*it)->checkForObservability(OBSERVATE_CLICKS))
				((IClickable*)(*it))->LeftClick(Mouse.Position);
			else 
				((IInteractive*)(*it))->mouseClicks(0,click,Mouse.Position);
		}
	}
	else if(Mouse.LEFT.RELEASE)
	{
		for(auto it = this->mouseClickListener.begin(); it != this->mouseClickListener.end(); ++it) 
		{
			if((*it)->checkForObservability(OBSERVATE_CLICKS))
				((IClickable*)(*it))->LeftRelease(Mouse.Position);
			else 
				((IInteractive*)(*it))->mouseClicks(0,click,Mouse.Position);
		}
	}

	if(click=Mouse.RIGHT.CLICK)
	{click=true;
		for(auto it = this->mouseClickListener.begin(); it != this->mouseClickListener.end(); ++it) 
		{
			if((*it)->checkForObservability(OBSERVATE_CLICKS))
				((IClickable*)(*it))->RightClick(Mouse.Position);
			else
				((IInteractive*)(*it))->mouseClicks(1,click,Mouse.Position);
		}
	}
	else if(Mouse.RIGHT.RELEASE)
	{
		for(auto it = this->mouseClickListener.begin(); it != this->mouseClickListener.end(); ++it) 
		{
			if((*it)->checkForObservability(OBSERVATE_CLICKS))
				((IClickable*)(*it))->LeftRelease(Mouse.Position);
			else 
				((IInteractive*)(*it))->mouseClicks(1,false,Mouse.Position);
		}
	}

	if(click=Mouse.MIDDLE.CLICK)
	{click=true;
		for(auto it = this->mouseClickListener.begin(); it != this->mouseClickListener.end(); ++it) 
		{
			if((*it)->checkForObservability(OBSERVATE_CLICKS))
				((IClickable*)(*it))->MiddleClick(Mouse.Position);
			else
				((IInteractive*)(*it))->mouseClicks(2,click,Mouse.Position);
		}
	}
	else if(Mouse.MIDDLE.RELEASE)
	{
		for(auto it = this->mouseClickListener.begin(); it != this->mouseClickListener.end(); ++it) 
		{
			if((*it)->checkForObservability(OBSERVATE_CLICKS))
				((IClickable*)(*it))->MiddleRelease(Mouse.Position);
			else 
				((IInteractive*)(*it))->mouseClicks(2,false,Mouse.Position);
		}
	}
}

void
InputManager::UpdateMouseButtons(int button,int state,int x,int y)
{
		switch (button)
		{
		case GLUT_LEFT_BUTTON:
			LEFTnewState = !state;
			break;
		case GLUT_MIDDLE_BUTTON:
			MIDDLEnewState =  !state;
			break;
		case GLUT_RIGHT_BUTTON:
			RIGHTnewState = !state;
			break;
		}
		setMousePosition(x,y);

		for(auto it=mouseClickListener.begin();it!=mouseClickListener.end();it++)
		{
			if((*it)->checkForObservability(OBSERVATE_KEYBOARD))
				((IInteractive*)(*it))->mouseClicks(button,state==GLUT_DOWN,Mouse.Position);
		}
}

void
InputManager::UpdateMouseWheel(int wheel,int state,int x,int y)
{
	// Resets The Mouse-Wheel variables to WHEEL::NONE... 
	// otherwhise the last detected UP or DOWN state will stay as state
	Mouse.WheelV = Mouse.WheelH = WHEEL::NONE;

	if(wheel == 0)			// the first mouswheel -> veretical scroll
		Mouse.WheelV = (WHEEL)state;
	else if(wheel == 1)		// the second mousewheel (if your Mouse has one...)
		Mouse.WheelH = (WHEEL)state;

//Output Status to Console if MOUSE_TEST_OUTPUT is defined...	
#ifdef MOUSE_TEST_OUTPUT 
	if(instance->Mouse.WheelV == WHEEL::UP)
		std::cout<<"\nWheelUP";
	if(instance->Mouse.WheelV == WHEEL::DOWN)
		std::cout<<"\nWheelDOWN";
#endif

// call the invoktionList
	notifyWheel(wheel);
}

void
InputManager::UpdateJoysticks(int id,unsigned buttons,int AxisX,int AxisY,int AxisZ)
{
	switch (id)
	{
	case 0:

		if(Controler1.Buttons!=buttons)
		{
			for(int i = 0;i<Controler1.NumberOfButtons;i++)
				if(_buttonChange[i] = buttons>>31-i)
					notifyJoystick(0,i,_buttonChange[i],Controler1.aX=AxisX,Controler1.aY=AxisY,Controler1.aZ=AxisZ);
		}
		else
		{
			_axisChange[0]=Controler1.aX!=AxisX;
			Controler1.aX = AxisX;
			_axisChange[1]=Controler1.aX!=AxisY;
			Controler1.aY = AxisY;
			_axisChange[2]=Controler1.aX!=AxisZ;
			Controler1.aZ = AxisZ;
			if(_axisChange[0]||_axisChange[1]||_axisChange[2])
				notifyJoystick(id,-1,NULL,Controler1.aX,Controler1.aY,Controler1.aZ);
		}
		break;

	case 1:
		//Todo: Implement second Joystick...
		break;
	}
}

glm::vec4 
InputManager::GetViewportRectangle(void)
{
	return Viewport;
}

void 
InputManager::SaveViewportRectangle(int x,int y,int w,int h)
{
	Viewport.x=x;
	Viewport.y=y;
	Viewport.w=w;
	Viewport.z=h;
}

void 
InputManager::SetDoubleclickTime(int milliseconds)
{
	_doubleClickLength = (DWORD)milliseconds;
}

void
InputManager::setMousePosition(int x,int y)
{
	Mouse.Movement.x = x-Mouse.Position.x;
	Mouse.Movement.y = y-Mouse.Position.y;
	Mouse.Position.x = Mouse.X = x;
	Mouse.Position.y = Mouse.Y = y;

	ViewPortNormalizedMouseCoordinates.x = 2*(Mouse.Position.x/GetViewportRectangle().w)-1;
	ViewPortNormalizedMouseCoordinates.y = 2*(1-Mouse.Position.y/GetViewportRectangle().z)-1;

	glm::vec4 ray_clip(ViewPortNormalizedMouseCoordinates.x,ViewPortNormalizedMouseCoordinates.y,-1,1);

/*
	
	glm::vec4 ray_eye = inverse (projection_matrix) * ray_clip;
	ray_eye = vec4 (ray_eye.xy, -1.0, 0.0);
	vec3 ray_wor = (inverse (view_matrix) * ray_eye).xyz;
// don't forget to normalise the vector at some point
	ray_wor = normalise (ray_wor);
	*/
}



void
InputManager::setMouseButtons(void)
{
	//evaluate Left-button-state:
	Mouse.LEFT.RELEASE = Mouse.LEFT.HOLD? !(Mouse.LEFT.HOLD = LEFTnewState):false;
	Mouse.LEFT.CLICK = (Mouse.LEFT.CLICK||Mouse.LEFT.DOUBLE)? !(Mouse.LEFT.HOLD = LEFTnewState): !Mouse.LEFT.HOLD? LEFTnewState:false;
	if(Mouse.LEFT.CLICK)
	{
		if(_clicktimer>0)
			Mouse.LEFT.CLICK = !(Mouse.LEFT.DOUBLE = true);
		_clicktimer = _doubleClickLength;
	}
	else
		Mouse.LEFT.DOUBLE=false;
	

	//evaluate Right-button-state:
	Mouse.RIGHT.RELEASE = Mouse.RIGHT.HOLD? !(Mouse.RIGHT.HOLD = RIGHTnewState):false;
	Mouse.RIGHT.CLICK = (Mouse.RIGHT.CLICK||Mouse.RIGHT.DOUBLE)? !(Mouse.RIGHT.HOLD = RIGHTnewState): !Mouse.RIGHT.HOLD? RIGHTnewState:false;
	if(Mouse.RIGHT.CLICK)
	{
		if(_clicktimer>0)
			Mouse.RIGHT.CLICK = !(Mouse.RIGHT.DOUBLE = true);
		_clicktimer = _doubleClickLength;
	}
	else
		Mouse.RIGHT.DOUBLE=false;

	//evaluate Middle-button-state:
	Mouse.MIDDLE.RELEASE = Mouse.MIDDLE.HOLD? !(Mouse.MIDDLE.HOLD = MIDDLEnewState):false;
	Mouse.MIDDLE.CLICK = (Mouse.MIDDLE.CLICK||Mouse.MIDDLE.DOUBLE)? !(Mouse.MIDDLE.HOLD = MIDDLEnewState): !Mouse.MIDDLE.HOLD? MIDDLEnewState:false;
	if(Mouse.MIDDLE.CLICK)
	{
		if(_clicktimer>0)
			Mouse.MIDDLE.CLICK = !(Mouse.MIDDLE.DOUBLE = true);
		_clicktimer = _doubleClickLength;
	}
	else
		Mouse.MIDDLE.DOUBLE=false;


	//reset Mouse-Wheels:
	Mouse.WheelV = Mouse.WheelH = WHEEL::NONE;


	//fire click-events if there where clicks during this frame...
	notifyQlicks();
}



void
InputManager::PerFrameReset(void)
{
	FrameTime = _updateTimer();

	setMouseButtons();


	#ifdef MOUSE_TEST_OUTPUT
		if(instance->Mouse.LEFT.CLICK)
			std::cout<<"CLICK\n";
		else if(instance->Mouse.LEFT.RELEASE)
			std::cout<<"\n...RELEASE\n\n";
		else if(instance->Mouse.LEFT.HOLD)
			std::cout<<"HOLD..";
		else if(instance->Mouse.LEFT.DOUBLE)
			std::cout<<"DOUBLE-CLICK!!!\n";

		if(instance->Mouse.RIGHT.CLICK)
			std::cout<<"RIGHT-CLICK\n";
		else if(instance->Mouse.RIGHT.RELEASE)
			std::cout<<"\n...RIGHT-RELEASE\n\n";
		else if(instance->Mouse.RIGHT.HOLD)
			std::cout<<"RIGHT-HOLD..";
		else if(instance->Mouse.RIGHT.DOUBLE)
			std::cout<<"DOUBLE-RIGHT!!!\n";

		if(instance->Mouse.MIDDLE.CLICK)
			std::cout<<"MIDDLE-CLICK\n";
		else if(instance->Mouse.MIDDLE.RELEASE)
			std::cout<<"\n...MIDDLE-RELEASE\n\n";
		else if(instance->Mouse.MIDDLE.HOLD)
			std::cout<<"MIDDLE-HOLD..";
		else if(instance->Mouse.MIDDLE.DOUBLE)
			std::cout<<"DOUBLE-MIDDLE!!!\n";
	#endif
}



bool
IObserver::checkForObservability(int flag)
{
	int obst=observedEvents();
	bool mouse = (obst/OBSERVATE_MOUSE) >= 1;
	obst %= OBSERVATE_MOUSE;
	bool keys = (obst/OBSERVATE_KEYBOARD) >=1;
	obst %= OBSERVATE_KEYBOARD;
	bool wheels = (obst/OBSERVATE_WHEELS) >=1;
	obst %= OBSERVATE_WHEELS;
	bool clicks = (obst/OBSERVATE_CLICKS) >=1;

	switch(flag)
	{
	case OBSERVATE_MOUSE:
		return mouse;
	case OBSERVATE_KEYBOARD:
		return keys;
	case OBSERVATE_WHEELS:
		return wheels;
	case OBSERVATE_CLICKS:
		return clicks;
	}

	return false;
}

IInteractive::IInteractive()
{
	
}
int
IInteractive::observedEvents()
{
	return OBSERVATE_MOUSE|OBSERVATE_KEYBOARD;
}

IClickable::IClickable()
{

}
int
IClickable::observedEvents()
{
	return OBSERVATE_CLICKS|OBSERVATE_MOUSE;
}
IWheelee::IWheelee()
{

}
int 
IWheelee::observedEvents()
{
	return OBSERVATE_MOUSE|OBSERVATE_WHEELS;
}