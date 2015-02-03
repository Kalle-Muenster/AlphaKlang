#ifndef _GUIMANAGER_H_
#define _GUIMANAGER_H_

#define MAX_NUM_GUI_OBJECTS 1024

//#ifndef _POJECT_MACROS__
//#define GUI GuiManager::getInstance()
//#endif

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
	ProjectMappe::Rectangle Area;
	virtual Vector3 move(Vector3);
	virtual Vector3 rotate(Vector3);
	virtual Vector3 scale(Vector3);
	bool UseTexture;
	bool IsActive;

	
};




class GuiManager
{
	class WriteOrder
	{
	public:
		const char* text;
		Vecti position;
		data32 color; 
		WriteOrder(const char*,short,short,data32);
	};
private:
	static bool NotIsInstanciated;
	GuiManager(void);
	List<IDrawable*,MAX_NUM_GUI_OBJECTS> elements;
	List<WriteOrder*,100> writeOrders;
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
	IObjection<IConnectable>* Element(string name);
	IObjection<IConnectable>* Element(GobID id);
	void DrawGUI(void);
	void Write(const char*,short,short,data32);
	
};

#endif
