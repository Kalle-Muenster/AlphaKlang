#ifndef __SCENEGRAPH__
#define __SCENEGRAPH__

class IGObject;
class Cam;


class SceneGraph
{
private:
	SceneGraph(void);
	std::vector<IGObject*> drawables;
	float r,g,b;
	void UpdateSky(void);

public:
	~SceneGraph(void);
	static SceneGraph* GetInstance();
	unsigned int Add(IGObject* object);
	void Remove(IGObject* object);
	void DrawAll();
	void DrawSky(void);
	Cam* camera;
	IGObject* Object(const char* name);
	IGObject* Object(unsigned int ID);
	unsigned ObjectsCount(void);
};

#endif