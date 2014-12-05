#include "projectGrafics.h"
#include "projectMacros.h"
#include "projectClasses.h"

#include "ShaderObj.h"
#include "Musikubus.h"
#include "ScreenOverlay.h"
#include "GuiObject.h"
#include "FogMachine.h"

//Global Declerations:
int wnd;
void* font;

//Objects:
SpectrumAnalyzer* analyzer;
Sprite* Framen;
GuiObject* guiding;
ScreenOverlay* overlay;

//Functions:
void InitGlut(void);
void LoadContent(void);
void GlInit(void);
void UpdateCycle(void);
void RenderCycle(void);
void OnDisplay(void);
void OnIdle(void);
void OnReshape(int,int);
void MouseMoveFunc(int,int);
void MouseClicks(int,int,int,int);
void MouseWheelFunc(int,int,int,int);
void processSpecialKeys(int,int,int);
void keyboardInput(unsigned char,int,int);
void keyboardUpInput(unsigned char,int,int);
//void MouseHoverWindow(int);
void GamePadFunc(unsigned,int,int,int);
int prepareForExit(void);
//void Render2D(void);

//Entrypoint:
int main(int argc,char** argv)
{
	glutInit(&argc,argv);

	InitGlut();
	GlInit();
	LoadContent();

	glutMainLoop();
	return prepareForExit();
}

int prepareForExit(void)
{
	//deletions:
	delete font;
	
	return EXIT_SUCCESS;
}

void InitGlut(void)
{
			
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowSize(SCREENWIDTH,SCREENHEIGHT);
	wnd = glutCreateWindow("-soundXgame-");
	font = GLUT_BITMAP_HELVETICA_18;

	glutDisplayFunc(OnDisplay);
	glutReshapeFunc(OnReshape);
	glutIdleFunc(OnIdle);

	glutMotionFunc(MouseMoveFunc);
	glutPassiveMotionFunc(MouseMoveFunc);
	glutMouseFunc(MouseClicks);
	glutMouseWheelFunc(MouseWheelFunc);
	glutSpecialFunc(processSpecialKeys);
	//glutEntryFunc(MouseHoverWindow);

	// Keyboard
	glutKeyboardFunc(keyboardInput);
	glutKeyboardUpFunc(keyboardUpInput);

	// hide mouse cursor
	glutSetCursor(GLUT_CURSOR_NONE); 
	
	GLenum glewError = glewInit();
	if( glewError != GLEW_OK )
	{
		std::cerr << "Unable to init GLew";
	}

}

void GlInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE); // unnötig hier, weil bereits in IMeshObject::draw() ???
	glCullFace(GL_BACK);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	
	
    glEnable(GL_MULTISAMPLE_ARB); // enable Multisample Antialiasing
	glEnable(GL_BLEND); // enable transparency

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // do transparency
}


ConID testID;
//int i1,i2,i3;
//int cycle1 = 127;
//int cycle2 = 255;
//int cycle3 = 127;
void LoadContent(void)
{
	
	//guiding = new GuiObject();
	//guiding->LoadTexture("testbild_1600x900.png");



	//i1 = -1;
	//i2 = -2;
	//i3 = 1;
	unsigned int brummsound;
//	AUDIO->LoadeSampleToBank(brummsound,"brumm_s16.wav");
	AUDIO->LoadeBackgroundAudio("testtrack.mp3");

//	AUDIO->Play();

	// Gameplay Objects
	Ground* ground = Ground::getInstance();
	//Framen = new Sprite("framen_1920x1080.png");
	Fountain* fountain = new Fountain();
	//ShaderObj* shaderObj = new ShaderObj();



	//(new Cubus("X-3.png", true, true))->SetName("Brummer");
	//SCENE->Object("Brummer")->GetConnected<AudioEmitter>()->PlaySample(AUDIO->GetSampleFromBank(brummsound),true);


	//Voxelplane... 
	//on runtime use press"X"to choose a an Image file from list (watch console!)
	//press "R" to loade the sellected image as Image.
	//press "L" to loade the sellected image'as Bumpmap. -> press several times each colorchannl seperate...
	VoxGrid* vObject = new VoxGrid("drei_.ppm");
	vObject->AddConnectable<VoxControl>(); // <-- look keyInput-function for more detail...
	vObject->GetConnected<VoxControl>()->Connection()->SetName("voxels");
	vObject->move(-80,0,150);
	vObject->MainSizzes.x=0.2;
	vObject->MainSizzes.y=0.045f;

	(new Cubus("X-7.png"))->SetName("AUDIO01");
	SCENE->Object("AUDIO01")->GetConnected<AudioEmitter>()->LoadeSample("DaRioGame v03 1-Kit-808.wav");
	SCENE->Object("AUDIO01")->move(2,0,-2);
	SCENE->Object("AUDIO01")->GetConnected<AudioEmitter>()->PlayAudio();
//	SCENE->Object("AUDIO01")->IsGrounded(true);
	(new Cubus("X-7.png"))->SetName("AUDIO02");
	SCENE->Object("AUDIO02")->GetConnected<AudioEmitter>()->LoadeSample("DaRioGame v03 2-Kit-808.wav");
	SCENE->Object("AUDIO02")->move(8,0,-2);
	SCENE->Object("AUDIO02")->GetConnected<AudioEmitter>()->PlayAudio();
//	SCENE->Object("AUDIO02")->IsGrounded(true);
	(new Cubus("X-7.png"))->SetName("AUDIO03");
	SCENE->Object("AUDIO03")->GetConnected<AudioEmitter>()->LoadeSample("DaRioGame v03 3-Kit-808.wav");
	SCENE->Object("AUDIO03")->move(12,0,-2);
	SCENE->Object("AUDIO03")->GetConnected<AudioEmitter>()->PlayAudio();
//	SCENE->Object("AUDIO03")->IsGrounded(true);
	(new Cubus("X-7.png"))->SetName("AUDIO04");
	SCENE->Object("AUDIO04")->GetConnected<AudioEmitter>()->LoadeSample("DaRioGame v03 4-Kit-808.wav");
	SCENE->Object("AUDIO04")->move(20,0,-2);
	SCENE->Object("AUDIO04")->GetConnected<AudioEmitter>()->PlayAudio();
//	SCENE->Object("AUDIO04")->IsGrounded(true);


	// Music Cube

	(new Cubus("Deckelblech-2s.png"))->SetName("muckubus");
	SCENE->Object("muckubus")->AddConnectable<Randomover>();
	SCENE->Object("muckubus")->AddConnectable<MusicListener>();
	SCENE->Object("muckubus")->GetConnected<MusicListener>()->GetLineData(0)->threshold = 1.5;
	SCENE->Object("muckubus")->GetConnected<MusicListener>()->GetLineData(1)->threshold = 0.03f;
	

	// Just some Music Cubes
	unsigned obj;
	float x,y,z;
	x=y=z=0;
	x = 15.0f;
	for(int i = 0;i<15;i++)
	{
		x-=((float)i);
		z+=(2.f*(float)i/15.0f);
		obj = (new Cubus("Deckelblech-2s.png"))->GetID();
		SCENE->Object(obj)->move(x,y,z);
		SCENE->Object(obj)->AddConnectable<Randomover>();
		SCENE->Object(obj)->GetConnected<Randomover>()->SetRotation(true);
		SCENE->Object(obj)->GetConnected<Randomover>()->SetMoving(true);
		SCENE->Object(obj)->GetConnected<AudioEmitter>()->LoadeSample("brumm_s16.wav");
		SCENE->Object(obj)->GetConnected<AudioEmitter>()->PlayAudio();
		SCENE->Object(obj)->AddConnectable<MusicListener>();
		SCENE->Object(obj)->AddConnectable<MusicListener>()->GetLineData(0)->fallOff;// += (float)i/100; 
		SCENE->Object(obj)->SetName("Brummer");
		SCENE->Object(obj)->IsVisible=true;
	}


	// Spectrum Analyzer
	analyzer = new SpectrumAnalyzer();
	analyzer->SetName("SpectrumAnalyzer");
	analyzer->AddConnectable<CamTargetRotator>();
	analyzer->move(0, 0, -88.0f);
	analyzer->scale(90.0f * 3.5f/128.0f, 0.3f, 2.0f); // 90 ground-tiles * 3.5m width * 128 bands
	analyzer->Initialize();

	(new FogMachine())->SetName("DasNebel");
	SCENE->Object("DasNebel")->move(10,0,5);
	((FogMachine*)SCENE->Object("DasNebel"))->MachDampf();

	// Camera
	SCENE->camera->Mode(FIRSTPERSON);
	SCENE->camera->SetTarget(SCENE->Object(obj));

	//overlay = new ScreenOverlay();
	//overlay->Initialize("framen_1920x1080.png");
}



GobID switcher=0;

//Main-Cycle:
////////////////////////////////////////////////////////
/* the Main-Updatecall */
void UpdateCycle(void)
{
	//color test flashing krams....
	//if(cycle1<0||cycle1>255)
	//	i1= -i1;
	//if(cycle2<0||cycle2>255)
	//	i2= -i2;
	//if(cycle3<0||cycle3>255)
	//	i3= -i3;

	//cycle1+=i1;
	//cycle2+=i2;
	//cycle3+=i3;

	//((Cubus*)SCENE->Object((unsigned)1))->color.byte[1] = cycle1;
	//((Cubus*)SCENE->Object((unsigned)1))->color.byte[2] = cycle2;
	//((Cubus*)SCENE->Object((unsigned)1))->color.byte[3] = cycle3;
	//((Cubus*)SCENE->Object((unsigned)1))->color.byte[0] = (255-cycle1);

	//data32 col = ((Cubus*)SCENE->Object((unsigned)0))->color;
	//printf("color: %i,%i,%i,%i\n",col.byte[1] ,col.byte[2] ,col.byte[3] ,col.byte[0] );
	//___________________________________________________________



	//Update:
	UPDATE->DoTheUpdates();


}
void DrawGui(void)
{
	 //	glMatrixMode(GL_PROJECTION); 
		//glPushMatrix(); 
		//glLoadIdentity(); 
		//gluOrtho2D(0,SCREENWIDTH,0,SCREENHEIGHT);

		//glBegin(GL_QUADS);

		//guiding->draw();
		//
		//glEnd();

		//glPopMatrix();
		//glMatrixMode(GL_MODELVIEW);
}
//The Main-Draw-Call...
void RenderCycle(void)
{
	SCENE->DrawSky();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SCENE->DrawAll();
	//Render2D();
	DrawGui();
	glutSwapBuffers();
}

//void Render2D(void)
//{
//	//glPushMatrix();
//
//
//
//	//glPopMatrix();
//}

//#define LATE_AFTER_DRAW
#define LATE_BEFOR_DRAW

//GL-DisplayCallbacks
////////////////////////////////////////////////////////

void OnDisplay(void)
{
	UpdateCycle();

	#ifdef LATE_BEFOR_DRAW
		UPDATE->DoTheLateUpdates();
	#endif

	RenderCycle();

	//Render2D();

	#ifdef LATE_AFTER_DRAW
		UPDATE->DoTheLateUpdates();
	#endif

	INPUT->PerFrameReset();
	AUDIO->PerFrameReset();
}

void OnIdle(void)
{
	OnDisplay();
}

void OnReshape(GLsizei size_x,GLsizei size_y)
{
	if(size_y == 0)
		size_y = 1;

	INPUT->SaveViewportRectangle(0,0,size_x,size_y);
	SCENE->camera->Aspect((float)size_x/(float)size_y);
}


//GL-InputFunctions:
///////////////////////////////////////////////////


//Keyboard:
void keyboardInput(unsigned char key,int x,int y)
{
	/* Switches Cam-Modes..*/
	if(key=='p')
	{	
		SCENE->camera->Mode(FIRSTPERSON);
	}
	if(key=='f')
	{	
		SCENE->camera->Mode(FOLLOWTARGET);
	}
	if(key=='o')
		SCENE->camera->Mode(SPECTATOR);

	if(key == 27) // ESC
		glutExit();

	INPUT->registerKey(key);
}

void keyboardUpInput(unsigned char key,int x,int y)
{
	INPUT->registerKeyUp(key);
}

void processSpecialKeys(int key, int xx, int yy)
{
	INPUT->notifySpecialKey(key);
}

//Mouse:
void MouseMoveFunc(int X,int Y)
{
	INPUT->notifyMouse(X,Y);
}

void MouseClicks(int button,int state,int X,int Y)
{
	INPUT->UpdateMouseButtons(button,state,X,Y);
}

void MouseWheelFunc(int wheel,int state,int X,int Y)
{
	INPUT->UpdateMouseWheel(wheel,state,X,Y);
}

/*void MouseHoverWindow(int)
{

}*/
