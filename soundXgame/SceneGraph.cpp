#include <vector>
#include "IGObject.h"
#include "Cam.h"
#include "SceneGraph.h"

#include <GL\glew.h>

SceneGraph* scenegraph;

SceneGraph::SceneGraph(void) : r(0), g(0), b(0)
{
	camera = new Cam();
	drawables = std::vector<IGObject*>();

}

SceneGraph::~SceneGraph(void)
{
	//delete cam;
}

SceneGraph*
SceneGraph::GetInstance()
{
	if(!scenegraph)
		scenegraph = new SceneGraph();
	return scenegraph;
}

unsigned int
SceneGraph::Add(IGObject* object)
{
	scenegraph->drawables.push_back(object);
	return (GobID)(scenegraph->drawables.size()-1);
}

void
SceneGraph::Remove(IGObject* object)
{
	//objects.erase(objects.begin());

	//scenegraph->drawables.push_back(object);
	//return (GobID)(scenegraph->drawables.size()-1);
}

void
SceneGraph::DrawAll()
{

	this->camera->Update();

	for(auto it = drawables.begin();it!=drawables.end();it++)
		(*it)->draw();
}

void
SceneGraph::DrawSky(void)
{
	//this->UpdateSky();

	glClearColor(r/255,g/255,b/255,1.);
}

void
SceneGraph::UpdateSky(void)
{
	float add = 1.0f;

	if(r < 255 && g == 0)
	{
		r += add;
		if(r > 255)
			r = 255;
	}
	else 
	{
		if(g < 255 && r == 255 && b == 0)
		{
			g += add;
			if(g > 255)
				g = 255;
		}
		else 
		{
			if (r > 0 && g == 255)
			{
				r -= add;
				if(r < 0)
					r = 0;
			}
			else 
			{
				if (b < 255 && r == 0)
				{
					b += add;
					if(b > 255)
						b = 255;
				}
				else 
				{
					if (g > 0)
					{
						g -= add;
						if(g < 0)
							g = 0;
					}
					else 
					{
						
						if (b > 0)
						{
							b -= add;
							if(b < 0)
								b = 0;
						}
						else
						{
							// end -> starting from scratch 
						}
					}
				} 
			} 
		}
	}
}

unsigned
SceneGraph::ObjectsCount(void)
{
	return drawables.size();
}

IGObject*
SceneGraph::Object(const char* name)
{
	for(auto it=drawables.begin();it!=drawables.end();it++)
		if((*it)->GetName()==name)
			return (*it);
	return NULL;
}

IGObject*
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