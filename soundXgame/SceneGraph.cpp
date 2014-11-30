#include <vector>
#include "IGObject.h"
#include "Cam.h"
#include "SceneGraph.h"

#include <GL\glew.h>



//IGObject* _drawables[MAXIMUM_SXCENE_OBJECT];
//unsigned _NumberOfSceneObjects = 0;

SceneGraph::SceneGraph(void) : r(0), g(0), b(0)
{
	SkyFlasherActive=false;
	camera = new Cam();
	VoxtructsInitiator::initiateVoxtructs();

	drawables = new LueckList<IGObject*,MAXIMUM_SXCENE_OBJECT>();

	//for(int i=0;i<MAXIMUM_SXCENE_OBJECT;i++)
	//	_drawables[i]=NULL;

	//_NumberOfSceneObjects=0;
}

SceneGraph::~SceneGraph(void)
{
	delete drawables;
}

GobID // Add's a gameobject to the Scene and returns it's given ID
SceneGraph::Add(IGObject* object)
{
	return drawables->Add(object);
}

void
SceneGraph::Remove(IGObject* object)
{
	drawables->Remove(object);
}

void
SceneGraph::DrawAll()
{
	camera->Update();
	for(unsigned ID = drawables->First(); ID<=drawables->Last(); ID=drawables->Next(ID))
	{
		if(drawables->at(ID)->IsVisible)
			drawables->at(ID)->draw();
	}
}


void
SceneGraph::DrawSky(void)
{
	if(SkyFlasherActive)
		this->UpdateSky();

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
	return drawables->Count();
}

IGObject* // Find a gameobject by it's Name...
SceneGraph::Object(const char* name)
{
	//check's the name case-sensitive...
	for(GobID ID = drawables->First(); ID <= drawables->Last(); ID = drawables->Next(ID))
		if(std::strcmp(drawables->at(ID)->GetName(),name)==0)
				return drawables->at(ID);

	//if "Name" not found,check's case-insensitive...
	for(GobID ID = drawables->First(); ID <= drawables->Last(); ID = drawables->Next(ID))
		if(_stricmp(drawables->at(ID)->GetName(),name)==0)
				return drawables->at(ID);

	//if "Name" not found...
	return NULL;
}


/* Find a gameobject by it's ID */
IGObject* 
SceneGraph::Object(GobID ID)
{
	if(drawables->at(ID)!=NULL)
	{
		if(ID == drawables->at(ID)->GetID())
			return drawables->at(ID);
	}
	
	return NULL;
}

void
SceneGraph::Destruct(GobID ID)
{
	if(drawables->at(ID)->GetID()==ID)
	{
			delete drawables->at(ID);
			drawables->Remove(ID);
	}
}

void 
SceneGraph::Destruct(IGObject* Obj)
{
	if(drawables->at(Obj->GetID()))
	{
		drawables->Remove(Obj->GetID());
		delete Obj;
	}
}

void 
SceneGraph::Destruct(string name)
{
	Destruct(Object(name));
}

GobID
SceneGraph::HighestID(void)
{
	return drawables->Last();
}