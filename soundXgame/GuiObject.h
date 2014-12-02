#ifndef _GUI_OBJECT_H_
#define _GUI_OBJECT_H_

#include "IMeshObject.h"
#include "DataStructs.h"
//#include "VoxelVector.h"

class GuiObject :
	public IGObject
{
protected:
	Texture texture;
	List<glm::vec3*,4> verts;
	List<glm::vec2*,4> uvs;
	data32 color;

public:
	GuiObject(void);
	virtual void Initialize(void);
	virtual ~GuiObject(void);
	virtual void draw(void);
	void LoadTexture(char*);
	ProjectMappe::Rectangle Position(float,float,float,float);
	ProjectMappe::Rectangle Position(ProjectMappe::Rectangle);
	bool UseTexture;
	bool IsActive;
};

#endif
