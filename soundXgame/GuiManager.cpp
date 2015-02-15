#include "GuiControlls.h"
#include "Utility.h"
#include "SceneGraph.h"
#include "InputManager.h"
#include "projectMacros.h"
#include "globalfunctions.h"
#include "Camera.h"
#include "FirstPerson.h"

data32 _color; 
Vecti fpsTextPosition = Vecti();
Vecti posTextPosition = Vecti();
char fpsSTRING[32];
char posSTRING[32];
float _gm_tc_r = 1;
float _gm_tc_g = 1;
float _gm_tc_b = 1;
float _gm_tc_a = 1;

//this defines "NOLL" - (a constant  union variable of 4bytes length containing the color 0x00000000 - which means "Transparent-Black" or No Color..)
#define NOLL *GuiManager::NoColor
 const data32 _noColor = {0x00000000};
 const data32* GuiManager::NoColor = &_noColor;


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
	 timeCounter += InputManager::getInstance()->FrameTime;
	 frameCounter++;

	 if(frameCounter>10)
	 {
		 sprintf_s(fpsSTRING, sizeof(fpsSTRING),"FPS: %f",((float)frameCounter/timeCounter));
		 timeCounter=0;
		 frameCounter=0;
	 }

	 _WriteText2D(&fpsSTRING[0],fpsTextPosition);

 }


bool
GuiManager::NotIsInstanciated = true;

bool _isAnyGUIActive = false;

GuiManager::GuiManager(void)
{
	NotIsInstanciated = false;
	scene = SceneGraph::getInstance();
	elements = List<GuiObject*,MAX_NUM_GUI_OBJECTS>();
	writeOrders = List<WriteOrder*,100>();
	//r=g=b=a=1.f;
	_color.byte[0] = 255;
	_color.byte[1] = 255;
	_color.byte[2] = 255;
	_color.byte[3] = 255;
	fpsTextPosition.ix = 20;
	fpsTextPosition.yps = 20;
	posTextPosition.ix = 300;
	posTextPosition.yps = 20;
	color.byte[0]=255;
	color.byte[1]=255;
	color.byte[2]=255;
	color.byte[3]=255;
	fpsTextPosition.ix=20;
	fpsTextPosition.yps=20;
}



GuiManager::~GuiManager(void)
{
	//if(elements.Count()>0)
	//	for(unsigned ID=elements.First();ID<=elements.Last();ID=elements.Next(ID))
	//		elements.Distruct(ID);
	if(writeOrders.Count()>0)
		for(unsigned ID=writeOrders.First();ID<=writeOrders.Last();ID=writeOrders.Next(ID))
			writeOrders.Distruct(ID);
}


 bool CAMERA_3D = true;
 void 
GuiManager::Enable2DDrawing(void)
{
	if(CAMERA_3D)
	{
		// Get a copy of the viewport
		GLint iViewport[4]; 
        glGetIntegerv( GL_VIEWPORT, iViewport );

        // Save a copy of the projection matrix so that we can restore it 
        // when it's time to do 3D rendering again.
        glMatrixMode( GL_PROJECTION );
        glPushMatrix();
        glLoadIdentity();

        // Set up the orthographic projection
        glOrtho(iViewport[0], iViewport[0]+iViewport[2], iViewport[1]+iViewport[3], iViewport[1], -1, 1 );

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




bool
GuiManager::IsAnyGUIActive(void)
{
   return _isAnyGUIActive;
}

void
GuiManager::Write(const char* Text,VectorF position,data32 coll)
{
	if(ProjectMappe::isGameRunning())
		writeOrders.Add(new WriteOrder(Text,position.x,position.y,coll));
}

void _writePosition()
{
	VectorF pos = SCENE->camera->ModeSocket->GetCameraMode<FirstPerson>()->GetPlayerPosition();

	sprintf_s(posSTRING, sizeof(posSTRING),"Pos: %i / %i \n",(int)pos.x, (int)pos.y);
	_WriteText2D(&posSTRING[0],posTextPosition,_color);
}

void
GuiManager::Write(const char* Text,short X,short Y,data32 Color)
{
	if(ProjectMappe::isGameRunning())
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
			bool SomeGUIWasDrawn = false;
			for(GobID ID = elements.First();ID<=elements.Last(); ID=elements.Next(ID))
				if(elements[ID]->isVisible())
				{
					elements[ID]->draw();
					SomeGUIWasDrawn = true;
				}
			_isAnyGUIActive = SomeGUIWasDrawn;
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
		{
			_writeFPS();
			_writePosition();
		}

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

 void
_backButtonClicked(IConnectable* sender)
{
	((ButtonControl*)sender)->Connection()->isVisible(false);
}

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
		GuiManager::getInstance()->Write(this->GetText(),Area.GetPosition()+VectorF(20,20),this->color);

}

GuiObject::GuiObject(void)
{
	ShowTitle=false;
	transform.scale = Vector3(1,1,1);
	Area = ProjectMappe::Rectangle(&transform.position.x,&transform.position.y,&transform.scale.x,&transform.scale.y);
	InitializeGUIObject();
	Label[0]='\0';
}


GuiObject::GuiObject(bool withBackButton)
{
	ShowTitle=false;
	transform.scale = Vector3(1,1,1);
	Area = ProjectMappe::Rectangle(&transform.position.x,&transform.position.y,&transform.scale.x,&transform.scale.y);
	InitializeGUIObject();
	SetUp("GUI/panel_256x256.png",false,false);
	Label[0]='\0';
	if(withBackButton)
		GenerateBackButton();
}

 GuiObject::GuiObject(const char* name,bool withBackButton)
{
	ShowTitle=false;
	transform.scale = Vector3(1,1,1);
	Area = ProjectMappe::Rectangle(&transform.position.x,&transform.position.y,&transform.scale.x,&transform.scale.y);
	InitializeGUIObject();
	SetUp(name,false,false);
	SetName(name);	
	Label[0]='\0';
	if(withBackButton)
		GenerateBackButton();
}

 void
 GuiObject::GenerateBackButton(void)
 {
	 ButtonControl* newcon = new ButtonControl();
	 newcon->ConnectionID = newcon->TypeHashCode = typeid(ButtonControl).hash_code();
	 newcon->SetConnection(this);
	 newcon->ConnectionID = 10;
	 conXtor->setConnectables(9,newcon);
	 newcon->SetText("   back");
	 conXtor->GetConnected<ButtonControl>(10)->PositionOnPanel = VectorF(15,15);
	 conXtor->GetConnected<ButtonControl>(10)->SizeScaledPanel = VectorF(0.4,0.1);
	 conXtor->GetConnected<ButtonControl>(10)->SetClickerFunc(_backButtonClicked);
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
	

	UseTexture = true;
	IsActive = true;
	SetID(GuiManager::getInstance()->Add(this));
	LockID();

}

void 
GuiObject::LoadTexture(char* filename)
{
	int i = 0;
	while((++i<64)&&(filename[i]!='_'));
	if(i<64)
		sscanf_s(&filename[i], "_%ix%i.",&texture.w,&texture.h, sizeof(Texture)*2);

	texture.ID = Utility::loadTexture(filename);
	transform.scale = Utility::GetScalevectorByAspect(texture.w,texture.h);
	
	IsActive = UseTexture = true;
	Area.SetSize(texture.w,texture.h);
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

void
GuiObject::ResetHard(bool leaveBackbutton)
{
	SetText("");
	char id = -1;
	char i = 0;
	int n = conXtor->NumberOfConnectedObjects;
	while(i<n)
	{id++;
		if(conXtor->getConnectables(id))
			{conXtor->RemoveConnected<IConnectable>(id); 
			i++;}
		else
			id++;}
	if(leaveBackbutton)
		GenerateBackButton();

//	this->Area.SetSize(200,80);
}

void GuiObject::SetText(const char * txt)
{
	char i = 0;
	while((Label[i]=txt[i])!='\0' && i<32)i++;
}

const char* 
GuiObject::GetText(void)
{
	return &Label[0];
}