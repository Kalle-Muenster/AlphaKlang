#ifndef _SLIDER_X_
#define _SLIDER_X_

#include "InputManager.h"
#include "Connectable.h"
#include "UpdateManager.h"
#include "GuiManager.h"

class SliderX :
	public IConnectable,
	public IUpdateble,
	public IInteractive,
	public IDrawable
{
private:
	glm::vec2 uvs[16];
	glm::vec3 verts[4];
	GLuint frameUVBuffers[4];
	GLuint vertexBufferID;
	Texture texture;
	char imageLayer;
	float left,right,top,bottom;
	bool XIsUnderControll,YIsUnderControll;
	bool DimensionsSwitched;
	VectorF lastMouse;
public:
	ProjectMappe::Rectangle Panel;
	ProjectMappe::Rectangle Area;
	Controlled<float> ValueX;
	Controlled<float> ValueY;
	SliderX(void);
	virtual ~SliderX(void);
	virtual bool Initialize(void);
	virtual void DoUpdate(void);
	virtual void draw(void);
	action ClickAction;
	VectorF PositionOnPanel;
	VectorF SizeScaledPanel;
	ProjectMappe::Rectangle GetArea(void);
	float angle;
	virtual void mouseMotion(int x, int y);
	virtual void mouseClicks(int button,bool IsPressed,VectorF position);

};

#endif