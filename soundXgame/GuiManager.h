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

//class GuiConXtor : public IConXtor
//{
//public:
//	virtual unsigned GetElementID(void);
//	virtual void draw(void);
//};

class GuiObject
	: public Sprite
{
private:
	char Label[32];
protected:
	void GenerateBackButton(void);
public:
	bool ShowTitle;
	GuiObject(void);
	GuiObject(bool);
	GuiObject(const char*,bool=false);
	void InitializeGUIObject(void);
	virtual ~GuiObject(void);
	void LoadTexture(char*);
	ProjectMappe::Rectangle Area;
	virtual Vector3 move(Vector3);
	virtual Vector3 rotate(Vector3);
	virtual Vector3 scale(Vector3);
	bool UseTexture;
	bool IsActive;
	virtual unsigned GetElementID(void);
	//operator GuiConXtor*(void);
	virtual void draw(void);
	virtual bool isVisible(BOOL=3);
	void ResetHard(bool leaveBackbutton = false);
	void SetText(const char * txt);
	const char* GetText(void);
	
};

class ControllElement;

#define NOLL *GuiManager::NoColor

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
	List<GuiObject*,MAX_NUM_GUI_OBJECTS> elements;
	List<WriteOrder*,100> writeOrders;
	data32 color;
	void Enable2DDrawing(void);
	void Disable2DDrawing(void);

public:
	static const data32 * NoColor;
	static const char* start(void);
	static GuiManager* getInstance(void);
	SceneGraph* scene;
	~GuiManager(void);
	GobID Add(GuiObject*);
	void Remove(GuiObject*);
	void Remove(unsigned ID);
	GuiObject* Panel(string name);
	GuiObject* Panel(GobID id);
	ControllElement* Element(string panelName,ConID ID);
	ControllElement* Element(unsigned panelID,unsigned elementID);
	void DrawGUI(void);
	void Write(const char*,short,short,data32=NOLL);
	void Write(const char*,VectorF,data32=NOLL);
	bool IsAnyGUIActive(void);
};

#endif
