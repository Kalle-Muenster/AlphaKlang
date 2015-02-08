#ifndef _KNOB_H_
#define _KNOB_H_

#include "controllelement.h"
#include "InputManager.h"

class Knob :
	public ControllElement,
	public IInteractive
{
private:
	glm::vec2 uvs[16];
	GLuint frameUVBuffers[4];
	char imageLayer;
	bool IsUnderControll;
	VectorF lastMouse;
	void _DrawBackground(void);
	void _DrawBar(float position);
	void _DrawForeground(void);

public:
	Knob(void);
	virtual ~Knob(void);
	static bool const canHaveMultipleInstances;
	Controlled<float> Value;
	virtual bool Initialize(void);
	virtual void DoUpdate(void);
	virtual void draw(void);
	action ClickAction;
	virtual ProjectMappe::Rectangle GetArea(void);

	virtual void mouseMotion(int x, int y);
	virtual void mouseClicks(int button,bool IsPressed,VectorF position);
};

#endif