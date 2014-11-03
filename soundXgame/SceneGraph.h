#ifndef __SCENEGRAPH__
#define __SCENEGRAPH__

class IGobject;
class Cam;


class SceneGraph
{
private:
	SceneGraph(void);
	std::vector<IGobject*> drawables;

public:
	~SceneGraph(void);
	static SceneGraph* GetInstance();
	unsigned int Add(IGobject*);
	void DrawAll();
	Cam* camera;
	IGobject* Object(const char* name);
	IGobject* Object(unsigned int ID);
	unsigned ObjectsCount(void);
};

#endif