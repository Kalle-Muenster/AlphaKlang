#include "GuiControlls.h"
#include "Utility.h"
#include "SceneGraph.h"
#include "InputManager.h"


 Vecti fpsTextPosition = Vecti();
 char fpsSTRING[32];
float _gm_tc_r = 1;
float _gm_tc_g = 1;
float _gm_tc_b = 1;
float _gm_tc_a = 1;

//this defines "NOLL" - (a constant  union variable of 4bytes length containing the color 0x00000000 - which means "Transparent-Black" or No Color..)
#define NOLL *GuiManager::NoColor
 const data32 _noColor = {0x00000000};
 const data32* GuiManager::NoColor = &_noColor;



bool
GuiManager::NotIsInstanciated = true;

GuiManager::GuiManager(void)
{
	NotIsInstanciated = false;
	scene = SceneGraph::getInstance();
	elements = List<GuiObject*,MAX_NUM_GUI_OBJECTS>();
	writeOrders = List<WriteOrder*,100>();
	color.byte[0]=255;
	color.byte[1]=255;
	color.byte[2]=255;
	color.byte[3]=255;
	fpsTextPosition.ix=20;
	fpsTextPosition.yps=20;
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
GuiManager::Add(GuiObject* panel)
{
	return elements.Add(panel) + MAX_MUM_SCENE_OBJECTS;
}

//GobID 
//GuiManager::Add(IConnectable* element)
//{
//	return elements.Add((IDrawable*)element) + MAX_MUM_SCENE_OBJECTS;
//}

void 
GuiManager::Remove(GuiObject* element)
{
	if( elements.Remove( element->GetID() - MAX_MUM_SCENE_OBJECTS) )
		return;
	else
	{
	for(GobID ID = elements.First();ID<=elements.Last(); ID=elements.Next(ID))
		if(elements[ID] == element)
		{
			elements.Remove(ID);
			return;
		}
	}
}

void 
GuiManager::Remove(unsigned elementID)
{
	elements.Remove(elementID-MAX_MUM_SCENE_OBJECTS);
}

GuiObject* 
GuiManager::Panel(string name)
{
	for(GobID ID = elements.First();ID<=elements.Last(); ID=elements.Next(ID))
		if(Utility::StringCompareRecursive(name,elements[ID]->GetName())>64)
			return (GuiObject*)elements[ID];
	return NULL;
}

ControllElement*
GuiManager::Element(string panelName,ConID ID)
{
	return Panel(panelName)->GetConnected<ControllElement>(ID);
}

ControllElement*
GuiManager::Element(unsigned panelID,unsigned elementID)
{
	return Panel(panelID)->GetConnected<ControllElement>(elementID);
}

GuiObject*
GuiManager::Panel(GobID id)
{
	GobID ID = id - MAX_MUM_SCENE_OBJECTS;
	if(elements[ID]->GetID()==id)
		return (GuiObject*)elements[ID];
	return NULL;
}


void _WriteText2D(const char * text, Vecti position,data32 color=NOLL)		
{
  unsigned short i=0;

  if(color.byte[0] == NULL)
	glColor4f(_gm_tc_r,_gm_tc_g,_gm_tc_b,_gm_tc_a);
  else
	glColor4f(color.byte[1]/255,color.byte[2]/255,color.byte[3]/255,color.byte[0]/255);

  glRasterPos2i(position.ix, position.yps);
  while(text[i] != '\0') 
  { glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, text[i++]);}
}



double timeCounter=0;
short frameCounter=0;
void _writeFPS()
{
	timeCounter +=	InputManager::getInstance()->FrameTime;
	frameCounter++;

	
	_WriteText2D(&fpsSTRING[0],fpsTextPosition);

	if(frameCounter>10)
	{
		sprintf(&fpsSTRING[0],"FPS: %f",((float)frameCounter/timeCounter));
		timeCounter=0;
		frameCounter=0;
	}
}

void
GuiManager::Write(const char* Text,VectorF position,data32 coll)
{
	writeOrders.Add(new WriteOrder(Text,position.x,position.y,coll));
}

void
GuiManager::Write(const char* Text,short X,short Y,data32 Color)
{
	writeOrders.Add(new WriteOrder(Text,X,Y,Color));
}

void
GuiManager::DrawGUI(void)
{	
	if(elements.Count()==0 && writeOrders.Count() == 0 && !scene->ShowFPS)
		return;

	Enable2DDrawing();
	{
		if(elements.Count()>0)
		{
			for(GobID ID = elements.First();ID<=elements.Last(); ID=elements.Next(ID))
				if(elements[ID]->isVisible())
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

		if(scene->ShowFPS)
			_writeFPS();
	}	
	Disable2DDrawing();
}
	  

GuiManager::WriteOrder::WriteOrder(const char* Text,short x,short y,data32 Color=NOLL)
{
	text = Text;
	position.ix = x;
	position.yps = y;
	color = Color;
}


#include "controllelement.h"

//unsigned
//GuiConXtor::GetElementID(void)
//{
//   return GetID();
//}
//
//void
//GuiConXtor::draw(void)
//{
//	Connection()->draw();
//}

void
GuiObject::draw(void)
{
	Sprite::draw();
	char i = -1;
	char id = i;
	while(++i < conXtor->NumberOfConnectedObjects)
	{id++;
		if(conXtor->getConnectables(id))
			((ControllElement*)conXtor->getConnectables(id))->draw();
		else
			id++;}
	if(ShowTitle)
		GuiManager::getInstance()->Write(this->GetName(),Area.GetPosition()+VectorF(20,20));

}

GuiObject::GuiObject(void)
{
	ShowTitle=false;
	transform.scale = Vector3(1,1,1);
	Area = ProjectMappe::Rectangle(&transform.position.x,&transform.position.y,&transform.scale.x,&transform.scale.y);
	InitializeGUIObject();
	 SetUp("panel_256x512.png",false,false);

}

//GuiObject::operator GuiConXtor*(void)
//{
//	 return (GuiConXtor*)conXtor;
//}

 GuiObject::GuiObject(string name)
{
	ShowTitle=false;
	transform.scale = Vector3(1,1,1);
	Area = ProjectMappe::Rectangle(&transform.position.x,&transform.position.y,&transform.scale.x,&transform.scale.y);
	InitializeGUIObject();
	SetUp(name,false,false);
	SetName(name);	
	
}

GuiObject::~GuiObject(void)
{
	delete conXtor;
}

unsigned
GuiObject::GetElementID(void)
{
	return conXtor->GetID();
}

void
GuiObject::InitializeGUIObject(void)
{
	Area.SetPosition(500,500);
	Area.SetSize(256,256);

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
	Area.SetPosition(m.x,m.y);
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
	VectorF vec = Area.GetSize();
	vec.x*=s.x;
	vec.y*=s.y;
	Area.SetSize(vec);
	return s;
}

bool
GuiObject::isVisible(BOOL value)
{
	if(value<3)
	{
		if(IsVisible!=value);
		{
			char i = -1;
			char id = -1;
			while(++i<conXtor->NumberOfConnectedObjects)
			{id++;
				if(conXtor->getConnectables(id))
					((ControllElement*)conXtor->getConnectables(id))->IsUpdatingActive=value;
				else
					id++;
			}
			IsVisible=value;
		}
	}
	return IsVisible;
}