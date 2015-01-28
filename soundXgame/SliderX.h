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
	GLuint frameUVBuffers[4];
	char imageLayer;
	bool XIsUnderControll,YIsUnderControll;
	bool DimensionsSwitched;
	VectorF lastMouse;
	void _DrawBackground(void);
	void _DrawBar(float position);
	void _DrawForeground(void);

public:

	Controlled<float> ValueX;
	Controlled<float> ValueY;
	SliderX(void);
	virtual ~SliderX(void);
	virtual bool Initialize(void);
	virtual void DoUpdate(void);
	virtual void draw(void);
	action ClickAction;
	virtual ProjectMappe::Rectangle GetArea(void);
	
	virtual void mouseMotion(int x, int y);
	virtual void mouseClicks(int button,bool IsPressed,VectorF position);

};

#endif