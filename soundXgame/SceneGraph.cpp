#include <vector>
#include "IGobject.h"
#include "Cam.h"
#include "SceneGraph.h"

SceneGraph* scenegraph;

SceneGraph::SceneGraph(void)
{
	camera = new Cam();
	drawables = std::vector<IGobject*>();
}

SceneGraph::~SceneGraph(void)
{
//	delete camera;
}

SceneGraph* SceneGraph::GetInstance()
{
	if(!scenegraph)
	{
		scenegraph = new SceneGraph();
	}
	return scenegraph;
}

void SceneGraph::Add(IGobject* object)
{
	scenegraph->drawables.push_back(object);
}



void SceneGraph::DrawAll()
{
	this->camera->Update();

	for(auto it = drawables.begin();it!=drawables.end();it++)
		(*it)->draw();
}