#include <vector>
#include "IGObject.h"
#include "Cam.h"
#include "SceneGraph.h"
#include <GL\glew.h>


// recursive string comperation... returns -1 if strings are equal, 
// else returns the number of character at wich they where different.
int 
_cmprStrngsRec(string A,string B,int position)
{
	if((A[position]=='\0')&&(B[position]=='\0'))
		return -1;
	else if(A[position]==B[position])
		return _cmprStrngsRec(A,B,++position);
	else
		return position;
}

//Compare 2 strings case sensetive by iteration..
bool
_compareStrings(string A,string B)
{
	bool result = false;
	int i = -1;
	while(A[++i]!='\0' && B[i]!='\0') 
	{
		if(A[i]==B[i])
			result = true;
		else
			return false;
	}
	return result;
}

//Compare 2 strings case sensitive by recursion...
bool 
_compareStringsRecursive(string A,string B)
{
	return _cmprStrngsRec(A,B,0)<0;
}





SceneGraph::SceneGraph(void) : r(0), g(0), b(0)
{
	SkyFlasherActive=false;
	camera = new Cam();
	VoxtructsInitiator::initiateVoxtructs();

	drawables = List<IGObject*,MAXIMUM_SXCENE_OBJECT>();
}

SceneGraph::~SceneGraph(void)
{
	for(int ID = drawables.First(); ID <= drawables.Last(); ID = drawables.Next(ID))
		drawables.Distruct(ID);

	//for(int ID = drawables.Last(); ID >= drawables.First(); ID = drawables.Prev(ID))
	//	if(ID!=0)
	//		drawables.Distruct(ID);
}

GobID // Add's a gameobject to the Scene and returns it's given ID
SceneGraph::Add(IGObject* object)
{
	return drawables.Add(object);
}

void
SceneGraph::Remove(IGObject* object)
{
	drawables.Remove(object);
}

void
SceneGraph::DrawAll()
{
	camera->Update();
	for(unsigned ID = drawables.First(); ID<=drawables.Last(); ID=drawables.Next(ID))
	{
		if(drawables[ID]->IsVisible)
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

IGObject* // Find a gameobject by it's Name...
SceneGraph::Object(char* name)
{
	//check's the name case-sensitive...
	for(GobID ID = drawables.First(); ID <= drawables.Last(); ID = drawables.Next(ID))
		if(_compareStringsRecursive(name,drawables[ID]->GetName()))
				return drawables[ID];

	//if "Name" not found,check's case-insensitive...
	for(GobID ID = drawables.First(); ID <= drawables.Last(); ID = drawables.Next(ID))
		if(_stricmp(drawables[ID]->GetName(),name)==0)
				return drawables[ID];

	//if "Name" not found...
	return NULL;
}

/* Find a gameobject by it's ID */
IGObject* 
SceneGraph::Object(GobID ID)
{
	if(drawables[ID]!=NULL)
	{
		if(ID == drawables[ID]->GetID())
			return drawables[ID];
	}
	
	return NULL;
}

/* Destroys a  GameObject by it's ID and remove's it from the List... */
void
SceneGraph::Destruct(GobID ID)
{
	if(drawables[ID]->GetID()==ID)
	{
			delete drawables[ID];
			drawables.Remove(ID);
	}
}

void 
SceneGraph::Destruct(IGObject* Obj)
{
	if(drawables[Obj->GetID()])
	{
		drawables.Remove(Obj->GetID());
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
	return drawables.Last();
}