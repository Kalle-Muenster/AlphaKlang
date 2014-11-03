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
	void Add(IGobject*);
	void DrawAll();
	Cam* camera;
};

#endif