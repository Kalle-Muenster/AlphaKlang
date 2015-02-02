#include <vector>
#include "IGObject.h"
#include "Cam.h"
#include "SceneGraph.h"
#include <GL\glew.h>


#include "GuiManager.h"


SceneGraph::SceneGraph(void) : r(0), g(0), b(0)
{
	SkyFlasherActive=false;
	camera = new Cam();
	VoxtructsInitiator::initiateVoxtructs();
	drawables = List<IDrawable*,MAX_MUM_SCENE_OBJECTS>();
}

SceneGraph::~SceneGraph(void)
{
	for(int ID = drawables.First(); ID <= drawables.Last(); ID = drawables.Next(ID))
	{
		drawables.Distruct(ID);
	}


	//for(int ID = drawables.Last(); ID >= drawables.First(); ID = drawables.Prev(ID))
	//	if(ID!=0)
	//		drawables.Distruct(ID);
}

GobID // Add's a gameobject to the Scene and returns it's given ID
SceneGraph::Add(IDrawable* object)
{
	return drawables.Add(object);
}

void
SceneGraph::Remove(IDrawable* object)
{
	drawables.Remove(object);
}

void
SceneGraph::DrawAll()
{
	camera->Update();
	for(unsigned ID = drawables.First(); ID<=drawables.Last(); ID=drawables.Next(ID))
	{
		if(drawables[ID]->isVisible())
			drawables[ID]->draw();
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
SceneGraph::DrawGUI(void)
{
	GuiManager::getInstance()->DrawGUI();
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
	return drawables.Count();
}

IObjection<IConnectable>* // Find a gameobject by it's Name...
SceneGraph::Object(char* name)
{
	//check's the name case-sensitive...
	for(GobID ID = drawables.First(); ID <= drawables.Last(); ID = drawables.Next(ID))
		if(Utility::StringCompareRecursive(name,((IObjection<IConXtor>*)drawables[ID])->GetName())>64)
				return(IObjection<IConnectable>*)drawables[ID];

	//if "Name" not found,check's case-insensitive...
	for(GobID ID = drawables.First(); ID <= drawables.Last(); ID = drawables.Next(ID))
		if(_stricmp(((IObjection<IConXtor>*)drawables[ID])->GetName(),name)==0)
				return (IObjection<IConnectable>*)drawables[ID];

	//if "Name" not found...
	return NULL;
}



/* Find a gameobject by it's ID */
IObjection<IConnectable>* 
SceneGraph::Object(GobID ID)
{
	if(drawables[ID]!=NULL)
	{
		if(ID == ((IGObject*)drawables[ID])->GetID())
			return (IObjection<IConnectable>*)(IGObject*)drawables[ID];
	}
	
	return NULL;
}

/* Destroys a  GameObject by it's ID and remove's it from the List... */
void
SceneGraph::Destruct(GobID ID)
{
	if(((IGObject*)drawables[ID])->GetID()==ID)
	{
			delete (IGObject*)drawables[ID];
			drawables.Remove(ID);
	}
}

void 
SceneGraph::Destruct(IDrawable* Obj)
{	unsigned id = ((IGObject*)Obj)->GetID();
	if(drawables[id])
	{
		drawables.Remove(id);
		delete (IGObject*)Obj;
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
	return drawables.Last();
}