#ifndef _CONTROLL_ELEMENT_H_
#define _CONTROLL_ELEMENT_H_

#include "Updatemanager.h"
#include "Connectable.h"
#include "IDrawable.h"
#include "GuiManager.h"

class ControllElement :
	public IUpdateble ,
	public IConnectable,
	public IDrawable
{
protected:
	Texture texture;
	float left,right,top,bottom;
	glm::vec3 verts[4];
	GLuint vertexBufferID;

public:
	ControllElement(void);
//	virtual bool Initialize(void);
	virtual ~ControllElement(void);
	ProjectMappe::Rectangle Panel;
	ProjectMappe::Rectangle Area;
	VectorF PositionOnPanel;
	VectorF SizeScaledPanel;
	virtual ProjectMappe::Rectangle GetArea(void);
	GLfloat angle;
	virtual bool isVisible(BOOL=3);
};

#endif