#include "GuiManager.h"
#include "Utility.h"
#include "SceneGraph.h"



bool
GuiManager::NotIsInstanciated = true;

GuiManager::GuiManager(void)
{
	NotIsInstanciated = false;
	scene = SceneGraph::getInstance();
	elements = List<IDrawable*,MAX_NUM_GUI_OBJECTS>();
	writeOrders = List<WriteOrder*,100>();
	r=g=b=a=1.f;
}



GuiManager::~GuiManager(void)
{
}


 bool CAMERA_3D = true;
 void 
GuiManager::Enable2DDrawing(void)
{
	if(CAMERA_3D)
	{
		GLint iViewport[4];



        // Get a copy of the viewport

        glGetIntegerv( GL_VIEWPORT, iViewport );



        // Save a copy of the projection matrix so that we can restore it 

        // when it's time to do 3D rendering again.

        glMatrixMode( GL_PROJECTION );

        glPushMatrix();

        glLoadIdentity();



        // Set up the orthographic projection

        glOrtho( iViewport[0], iViewport[0]+iViewport[2],

                         iViewport[1]+iViewport[3], iViewport[1], -1, 1 );

        glMatrixMode( GL_MODELVIEW );

        glPushMatrix();

        glLoadIdentity();



        // Make sure depth testing and lighting are disabled for 2D rendering until

        // we are finished rendering in 2D

        glPushAttrib( GL_DEPTH_BUFFER_BIT | GL_LIGHTING_BIT );

        glDisable( GL_DEPTH_TEST );

        glDisable( GL_LIGHTING );

		CAMERA_3D = false;
	}
}

void 
GuiManager::Disable2DDrawing(void)
{
	if(!CAMERA_3D)
	{
 	 glPopAttrib();

        glMatrixMode( GL_PROJECTION );

        glPopMatrix();

        glMatrixMode( GL_MODELVIEW );

        glPopMatrix();

		CAMERA_3D = true;
	}
}





GobID 
GuiManager::Add(IDrawable* element)
{
	return elements.Add(element) + MAX_MUM_SCENE_OBJECTS;
}

void 
GuiManager::Remove(IDrawable* element)
{
	for(GobID ID = elements.First();ID<=elements.Last(); ID=elements.Next(ID))
		if(elements[ID] == element)
			elements.Remove(ID);
}

IObjection<Connectable<IDrawable>>* 
GuiManager::Element(string name)
{
	for(GobID ID = elements.First();ID<=elements.Last(); ID=elements.Next(ID))
		if(Utility::StringCompareRecursive(((IObjection<IConnectable>*)elements[ID])->GetName(),name))
			return (IObjection<Connectable<IDrawable>>*)elements[ID];
	return NULL;
}

IObjection<Connectable<IDrawable>>*
GuiManager::Element(GobID id)
{
	GobID ID = id - MAX_MUM_SCENE_OBJECTS;
	if(((IObjection<IConnectable>*)elements[ID])->GetID()==id)
		return (IObjection<Connectable<IDrawable>>*)elements[ID];
	return NULL;
}


void _WriteText2D(const char * text, Vecti position,data32 color)		
{
  unsigned short i=0;
  glColor4f(color.byte[0]/255,color.byte[1]/255,color.byte[2]/255,color.byte[3]/255);
  glRasterPos2i(position.ix, position.yps);
  while(text[i] != '\0') 
  { glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i++]);}
}

void
GuiManager::Write(const char* Text,short X,short Y,unsigned Color)
{
	data32 col;
	col.u32 = Color;
	writeOrders.Add(new WriteOrder(Text,X,Y,col));
}

void
GuiManager::DrawGUI(void)
{
	Enable2DDrawing();
	{
		if(elements.Count()>0)
		{
			for(GobID ID = elements.First();ID<=elements.Last(); ID=elements.Next(ID))
				if(elements[ID]->IsVisible)
					elements[ID]->draw();
		}

		if(writeOrders.Count()>0)
		{
			for(unsigned ID = writeOrders.First();ID<=writeOrders.Last();ID=writeOrders.Next(ID))
			{
				_WriteText2D(writeOrders[ID]->text,writeOrders[ID]->position,writeOrders[ID]->color);
				writeOrders.Distruct(ID);
			}
		}
	}	
	Disable2DDrawing();
}

GuiManager::WriteOrder::WriteOrder(const char* Text,short x,short y,data32 Color)
{
	text = Text;
	position.ix = x;
	position.yps = y;
	color = Color;
}

GuiObject::GuiObject(void)
{
	transform.scale = Vector3(1,1,1);
	area = ProjectMappe::Rectangle(&transform.position.x,&transform.position.y,&transform.scale.x,&transform.scale.y);
	InitializeGUIObject();
	 SetUp("testbild_1600x900.png",false,false);

}

 GuiObject::GuiObject(string name)
{
	transform.scale = Vector3(1,1,1);
	area = ProjectMappe::Rectangle(&transform.position.x,&transform.position.y,&transform.scale.x,&transform.scale.y);
	InitializeGUIObject();
	SetUp(name,false,false);
	SetName(name);	
	
}

GuiObject::~GuiObject(void)
{

}

void
GuiObject::InitializeGUIObject(void)
{
	area.SetPosition(500,500);
	area.SetSize(256,256);

	UseTexture = true;
	IsVisible = IsActive = true;
	SetID(GuiManager::getInstance()->Add(this));
	LockID();
}



void 
GuiObject::LoadTexture(char* filename)
{


	int i = 0;
	while((++i<64)&&(filename[i]!='_'));
	if(i<64)
		sscanf(&filename[i],"_%ix%i.",&texture.w,&texture.h);

	texture.ID = Utility::loadTexture(filename);
	transform.scale = Utility::GetScalevectorByAspect(texture.w,texture.h);
	
	

	IsVisible = true;
		IsActive = UseTexture = true;
}
	

Vector3 
GuiObject::move(Vector3 m)
{
	area.SetPosition(m.x,m.y);
	return m;
}

Vector3 
GuiObject::rotate(Vector3 r)
{
	angle = r.z;
	this->getTransform()->rotation = this->getTransform()->forward;
	return r;
}

Vector3
GuiObject::scale(Vector3 s)
{
	VectorF vec = area.GetSize();
	vec.x*=s.x;
	vec.y*=s.y;
	area.SetSize(vec);
	return s;
}