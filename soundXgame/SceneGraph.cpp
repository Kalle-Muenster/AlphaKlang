#include "SceneGraph.h"

SceneGraph* scenegraph;

SceneGraph::SceneGraph(void)
{
	camera = new Cam();
	drawables = std::vector<AbsGobject*>();
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

void SceneGraph::Add(AbsGobject* object)
{
	scenegraph->drawables.push_back(object);
}



void SceneGraph::DrawAll()
{
	this->camera->Update();

	for(auto it = drawables.begin();it!=drawables.end();it++)
		(*it)->draw();
}