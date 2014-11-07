#ifndef __SCENEGRAPH__
#define __SCENEGRAPH__

class IGobject;
class Cam;


class SceneGraph
{
private:
	SceneGraph(void);
	std::vector<IGobject*> drawables;
	float r,g,b;
	void UpdateSky(void);

public:
	~SceneGraph(void);
	static SceneGraph* GetInstance();
	unsigned int Add(IGobject*);
	void DrawAll();
	void DrawSky(void);
	Cam* camera;
	IGobject* Object(const char* name);
	IGobject* Object(unsigned int ID);
	unsigned ObjectsCount(void);
};

#endif