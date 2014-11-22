#include <vector>
#include "IGObject.h"
#include "Cam.h"
#include "SceneGraph.h"

#include <GL\glew.h>

SceneGraph* scenegraph;

IGObject* _drawables[MAXIMUM_SXCENE_OBJECT];
unsigned _NumberOfSceneObjects = 0;

SceneGraph::SceneGraph(void) : r(0), g(0), b(0)
{
	camera = new Cam();
//	drawables = std::vector<IGObject*>();

	for(int i=0;i<MAXIMUM_SXCENE_OBJECT;i++)
		_drawables[i]=NULL;

	_NumberOfSceneObjects=0;
}

SceneGraph::~SceneGraph(void)
{
	delete[] _drawables;
}

SceneGraph*
SceneGraph::GetInstance()
{
	if(!scenegraph)
		scenegraph = new SceneGraph();
	return scenegraph;
}

GobID // Add's a gameobject to the Scene and returns it's given ID
SceneGraph::Add(IGObject* object)
{
	unsigned counter = 0;
	unsigned i = 0;
	int	FreeSlotFound = -1;

	//check if the object is already in scenegraph... 
	while(counter<_NumberOfSceneObjects)
	{ //will remember if there where free slots, if some object has been removed before...
		if(_drawables[i]==NULL)
			FreeSlotFound = FreeSlotFound<0? i : FreeSlotFound;
		else if(_drawables[i]==object)
			return i; //returns if list already contains the object.. 
		else 
			counter++;
		i++; //count up next...
	}

	if(FreeSlotFound>=0)
	{// if found a free slot before:
		_drawables[FreeSlotFound] = object;
		_NumberOfSceneObjects++;
		return FreeSlotFound;
	}
	else for(i=_NumberOfSceneObjects;i<MAXIMUM_SXCENE_OBJECT;i++)
	{// else searches the list for next free slot...
		if(_drawables[i]==NULL)
		{
			_drawables[i]=object;
			_NumberOfSceneObjects++;
			return i;
		}
	}

//	scenegraph->drawables.push_back(object);
//	return (GobID)(scenegraph->drawables.size()-1);
}

void
SceneGraph::Remove(IGObject* object)
{
	if(_drawables[object->GetID()]==object)
	{
		_drawables[object->GetID()]==NULL;
		_NumberOfSceneObjects--;
	}
	//objects.erase(objects.begin());

	//scenegraph->drawables.push_back(object);
	//return (GobID)(scenegraph->drawables.size()-1);
}

void
SceneGraph::DrawAll()
{
	unsigned counter=0;
	unsigned index=0;

	this->camera->Update();

	while(counter<_NumberOfSceneObjects)
	{
		if(_drawables[index]!=NULL)
		{counter++;
			if(_drawables[index]->IsVisible)
				_drawables[index]->draw();
		}
	++index;
	}

	//for(auto it = drawables.begin();it!=drawables.end();it++)
	//	if((*it)->IsVisible)
	//		(*it)->draw();
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
//	return drawables.size();
	return _NumberOfSceneObjects;
}

IGObject* // Find a gameobject by it's Name...
SceneGraph::Object(const char* name)
{
	unsigned counter = 0;
	unsigned index = 0;
	
	while(counter<_NumberOfSceneObjects)
	{
		if(_drawables[index] != NULL)
		{counter++;
			if(_drawables[index]->GetName() == name)
				return _drawables[index];
		}
		index++;
	}
	//for(auto it=drawables.begin();it!=drawables.end();it++)
	//	if((*it)->GetName()==name)
	//		return (*it);
	return NULL;
}

IGObject* // Find a gameobject by it's ID
SceneGraph::Object(GobID ID)
{
	//if(drawables.at(ID)->GetID()==ID)
	//	return drawables.at(ID);
	//else
	//for(auto it=drawables.begin();it!=drawables.end();it++)
	//	if((*it)->GetID()==ID)
	//		return (*it);

	if(ID == _drawables[ID]->GetID())
		return _drawables[ID];
	else
		return NULL;
}