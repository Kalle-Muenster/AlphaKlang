#ifndef _CONNECTALE_BUTTON_
#define	_CONNECTALE_BUTTON_

#include "updatemanager.h"
#include "Connectable.h"

#include "IDrawable.h"






class ButtonControl :
	public IConnectable,
	public IDrawable, 
	public IUpdateble
{
private:
	std::vector<glm::vec2> uvs;
	glm::vec3 verts[4];
	GLuint frameUVBuffers[4];
	GLuint vertexBufferID;
	Texture texture;
	char btnState;
	float left,right,top,bottom;

public:
	static const char* States[];
	enum ButtonState : char
	{DISABLED=0,NORMAL=1,HOVER=2,PRESSED=3};
	ButtonControl(void);
	virtual void DoLate(void);
	virtual ~ButtonControl(void);
	virtual bool Initialize(void);
	virtual ButtonState GetState(void);
	virtual void SetState(ButtonState);
	void SetClickerFunc(action);
	action ClickAction;
	virtual void draw(void);
	ProjectMappe::Rectangle	Panel;
	VectorF PositionOnPanel;
	VectorF SizeScaledPanel;
	ProjectMappe::Rectangle GetArea(void);
	GLfloat angle;
};

#endif
