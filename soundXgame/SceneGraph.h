#ifndef __SCENEGRAPH__
#define __SCENEGRAPH__
#include <vector>
#include "IGobject.h"
#include "Cam.h"

class SceneGraph
{
private:
	SceneGraph(void);
	
	std::vector<AbsGobject*> drawables;
public:
	~SceneGraph(void);
	static SceneGraph* GetInstance();
	void Add(AbsGobject*);
	void DrawAll();
	Cam* camera;
};

#endif