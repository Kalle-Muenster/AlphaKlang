#ifndef _GUIMANAGER_H_
#define _GUIMANAGER_H_

#define MAX_NUM_GUI_OBJECTS 1024

#include "DataStructs.h"
#include "Sprite.h"
#include "list.h"


class SceneGraph;

class GuiObject
	: public Sprite
{
public:
	
	GuiObject(void);
	GuiObject(char*);
	void InitializeGUIObject(void);
	virtual ~GuiObject(void);
	void LoadTexture(char*);
	ProjectMappe::Rectangle area;
	virtual Vector3 move(Vector3);
	virtual Vector3 rotate(Vector3);
	virtual Vector3 scale(Vector3);
	bool UseTexture;
	bool IsActive;
};




class GuiManager
{
private:
	static bool NotIsInstanciated;
	GuiManager(void);
	List<IDrawable*,MAX_NUM_GUI_OBJECTS> elements;
	float r,g,b,a;
	void Enable2DDrawing(void);
	void Disable2DDrawing(void);

public:
	static const char* start(void);
	static GuiManager* getInstance(void);
	SceneGraph* scene;
	~GuiManager(void);
	GobID Add(IDrawable*);
	void Remove(IDrawable*);
	IObjection<Connectable<IDrawable>>* Element(string name);
	IObjection<Connectable<IDrawable>>* Element(GobID id);
	void DrawGUI(void);
};

#endif
