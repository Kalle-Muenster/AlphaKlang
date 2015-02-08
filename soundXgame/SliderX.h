#ifndef _SLIDER_X_
#define _SLIDER_X_

#include "ControllElement.h"
#include "InputManager.h"

class SliderX :
	public ControllElement,
	public IInteractive

{
private:
	glm::vec2 uvs[16];
	GLuint frameUVBuffers[2];
	char imageLayer;
	bool XIsUnderControll,YIsUnderControll;
	bool DimensionsSwitched;
	VectorF lastMouse;
	void _DrawBackground(void);
	void _DrawBar(short i,float position,float move);
	void _DrawForeground(void);

public:
	static bool const canHaveMultipleInstances;
	Controlled<float> ValueX;
	Controlled<float> ValueY;
	SliderX(void);
	virtual ~SliderX(void);
	virtual void DoUpdate(void);
	virtual void draw(void);
	action ClickAction;
	virtual ProjectMappe::Rectangle GetArea(void);
	
	virtual void mouseMotion(int x, int y);
	virtual void mouseClicks(int button,bool IsPressed,VectorF position);

};

#endif