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
	//delete camera;
}

SceneGraph* SceneGraph::GetInstance()
{
	if(!scenegraph)
	{
		scenegraph = new SceneGraph();
	}
	return scenegraph;
}

unsigned int SceneGraph::Add(IGobject* object)
{
	scenegraph->drawables.push_back(object);
	return (GobID)(scenegraph->drawables.size()-1);
}



void SceneGraph::DrawAll()
{
	this->camera->Update();

	for(auto it = drawables.begin();it!=drawables.end();it++)
		(*it)->draw();
}

unsigned
SceneGraph::ObjectsCount(void)
{
	return drawables.size();
}

IGobject*
SceneGraph::Object(const char* name)
{
	for(auto it=drawables.begin();it!=drawables.end();it++)
		if((*it)->GetName()==name)
			return (*it);
	return NULL;
}

IGobject*
SceneGraph::Object(unsigned int ID)
{
	if(drawables.at(ID)->GetID()==ID)
		return drawables.at(ID);
	else
	for(auto it=drawables.begin();it!=drawables.end();it++)
		if((*it)->GetID()==ID)
			return (*it);
	return NULL;
}