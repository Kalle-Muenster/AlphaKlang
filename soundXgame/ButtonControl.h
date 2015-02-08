#ifndef _CONNECTALE_BUTTON_
#define	_CONNECTALE_BUTTON_

#include "ControllElement.h"

class ButtonControl :
	public ControllElement
{
private:
	std::vector<glm::vec2> uvs;
	GLuint frameUVBuffers[4];
	char btnState;
	bool* Switch;

public:
	static bool const canHaveMultipleInstances;
	static const char* States[];
	enum ButtonState : char
	{DISABLED=0,NORMAL=1,HOVER=2,PRESSED=3};
	ButtonControl(void);
	virtual void DoUpdate(void);
	virtual ~ButtonControl(void);
//	virtual bool Initialize(void);
	virtual ButtonState GetState(void);
	void SetState(ButtonState);
	void SetClickerFunc(action);
	void SetSwitch(bool*);
	action ClickAction;
	virtual void draw(void);
//	virtual ProjectMappe::Rectangle GetArea(void);

};

#endif
