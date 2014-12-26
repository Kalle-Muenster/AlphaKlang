#include "projectGrafics.h"
#include "projectMacros.h"
#include "projectClasses.h"

#include "ShaderObj.h"
#include "Musikubus.h"
#include "ScreenOverlay.h"
#include "GuiObject.h"
#include "ParticleSystem.h"


#include <xercesc\dom\DOM.hpp>
#include <xercesc\framework\LocalFileInputSource.hpp>

#include "MusicInteractor.h"
#include "MusicScaler.h"
#include "Transform.h"

//Global Declerations:
int wnd;
void* font;
bool EXIT = false; 
//Objects:
//SpectrumAnalyzer* analyzer;
Sprite* Framen;
GuiObject* guiding;
ScreenOverlay* overlay;
//Fountain* fountain;
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
	glutExit();
	//deletions:
	ProjectMappe::GlobalDestructor();
	//delete analyzer;
	delete Framen;
	delete overlay; 
	delete guiding;
	font = NULL;
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
	
	AUDIO->Set3D_DopplerFXFactor(0.25f);
	AUDIO->Set3D_DistanceFactor(0.75f);
	AUDIO->Volume(0);
	//i1 = -1;
	//i2 = -2;
	//i3 = 1;
	unsigned int brummsound;
//	AUDIO->LoadeSampleToBank(brummsound,"brumm_s16.wav");

//	AUDIO->LoadeBackgroundAudio("DaRioGame v03.wav");
//	AUDIO->Play();
//	AUDIO->BackgroundMusicVolume(0.1);

	// Gameplay Objects
	Ground* ground = Ground::getInstance();
	//Framen = new Sprite("framen_1920x1080.png");
	
	//Fountain* fountain = new Fountain();
	//fountain->SetPosition(Vector3(-40, 0, 0));
	//fountain->size = 50;
	//fountain->createRange();

	//fountain->SetLineBounds(0,0,4,3);
	//fountain->SetClambt(0,-1,1);
	//fountain->SetThreshold(0,0.33);
	//fountain->sensitivity = 5;

	//ShaderObj* shaderObj = new ShaderObj();



	//(new Cubus("X-3.png", true, true))->SetName("Brummer");
	//SCENE->Object("Brummer")->GetConnected<AudioEmitter>()->PlaySample(AUDIO->GetSampleFromBank(brummsound),true);
	(new SpriteAnimation<36,3>("Q2_1872x516.png",12,3,25,true))->SetName("Q2animated");
	SCENE->Object("Q2animated")->move(0,0,0);
	//Voxelplane... 
	//on runtime use press"X"to choose a an Image file from list (watch console!)
	//press "R" to loade the sellected image as Image.
	//press "L" to loade the sellected image'as Bumpmap. -> press several times each colorchannl seperate...
	VoxGrid* vObject = new VoxGrid("drei_.ppm");
	vObject->AddConnectable<VoxControl>(); // <-- look keyInput-function for more detail...
	vObject->GetConnected<VoxControl>()->Connection()->SetName("voxels");
	vObject->AddConnectable<MusicVox>();
	vObject->move(-80,0,150);
	vObject->MainSizzes.x=0.2;
	vObject->MainSizzes.y=0.045f;

	GobID vox2 = (new VoxGrid("buntbild_128.ppm"))->GetID();
	SCENE->Object(vox2)->SetName("VoxelPlane2");
	SCENE->Object(vox2)->AddConnectable<VoxControl>();
	SCENE->Object(vox2)->AddConnectable<MusicVox>();
	((VoxGrid*)SCENE->Object(vox2))->flip(Vector3(1,0,0));

	
	(new Cubus("X-7.png"))->SetName("AUDIO01");
	SCENE->Object("AUDIO01")->GetConnected<AudioEmitter>()->LoadeSample("mp3/15-Audio.mp3");
	SCENE->Object("AUDIO01")->move(-20,0,-4);
	SCENE->Object("AUDIO01")->AddConnectable<MusicScaler>();
	SCENE->Object("AUDIO01")->IsGrounded(true);
	IMeshObject* meshObj;
	meshObj = static_cast<IMeshObject*>(SCENE->Object("AUDIO01"));	
	meshObj->GroundedWithPivot = true;

	(new Cubus("X-7.png"))->SetName("AUDIO02");
	SCENE->Object("AUDIO02")->GetConnected<AudioEmitter>()->LoadeSample("mp3/10-Nanopad.mp3");
	SCENE->Object("AUDIO02")->move(8,0,-2);
	SCENE->Object("AUDIO02")->AddConnectable<MusicScaler>();
	SCENE->Object("AUDIO02")->IsGrounded(true);

	(new Cubus("X-7.png"))->SetName("AUDIO03");
	SCENE->Object("AUDIO03")->GetConnected<AudioEmitter>()->LoadeSample("mp3/11-Audio.mp3");
	SCENE->Object("AUDIO03")->move(12,0,-2);
	SCENE->Object("AUDIO03")->AddConnectable<MusicScaler>();
	SCENE->Object("AUDIO03")->IsGrounded(true);

	(new Cubus("X-7.png"))->SetName("AUDIO04");
	SCENE->Object("AUDIO04")->GetConnected<AudioEmitter>()->LoadeSample("mp3/18-Audio.mp3");
	SCENE->Object("AUDIO04")->move(20,0,-2);
	SCENE->Object("AUDIO04")->IsGrounded(true);

	(new Cubus("X-7.png"))->SetName("AUDIO05");
	SCENE->Object("AUDIO05")->GetConnected<AudioEmitter>()->LoadeSample("mp3/16-Audio.mp3");
	SCENE->Object("AUDIO05")->move(2,0,-10);

	(new Cubus("X-7.png"))->SetName("AUDIO06");
	SCENE->Object("AUDIO06")->GetConnected<AudioEmitter>()->LoadeSample("mp3/06-CZ_ToogBass.mp3");
	SCENE->Object("AUDIO06")->move(8,0,-10);
	SCENE->Object("AUDIO06")->AddConnectable<MusicScaler>();
	SCENE->Object("AUDIO06")->GetConnected<MusicScaler>()->sensitivity=15;
	SCENE->Object("AUDIO06")->GetConnected<MusicScaler>()->SetClambt(0,-1.1);
	SCENE->Object("AUDIO06")->GetConnected<MusicScaler>()->SetThreshold(0,0.0002f);

	(new Cubus("X-7.png"))->SetName("AUDIO07");
	SCENE->Object("AUDIO07")->GetConnected<AudioEmitter>()->LoadeSample("mp3/19-Audio.mp3");
	SCENE->Object("AUDIO07")->move(12,0,-10);

	(new Cubus("X-7.png"))->SetName("AUDIO08");
	SCENE->Object("AUDIO08")->GetConnected<AudioEmitter>()->LoadeSample("mp3/21-Audio.mp3");
	SCENE->Object("AUDIO08")->move(20,0,-10);

	(new Cubus("X-7.png"))->SetName("AUDIO09");
	SCENE->Object("AUDIO09")->GetConnected<AudioEmitter>()->LoadeSample("mp3/08-Sforzando.mp3");
	SCENE->Object("AUDIO09")->move(2,0,-18);

	(new Cubus("X-7.png",true,true))->SetName("AUDIO10");
	SCENE->Object("AUDIO10")->GetConnected<AudioEmitter>()->LoadeSample("mp3/12-Audio.mp3");
	SCENE->Object("AUDIO10")->move(8,0,-18);
	SCENE->Object("AUDIO10")->AddConnectable<MusicScaler>();
	SCENE->Object("AUDIO10")->GetConnected<MusicScaler>()->sensitivity=2;
	SCENE->Object("AUDIO06")->GetConnected<MusicScaler>()->SetThreshold(0,0.02f);

	(new Cubus("X-7.png"))->SetName("AUDIO11");
	SCENE->Object("AUDIO11")->GetConnected<AudioEmitter>()->LoadeSample("mp3/05-TeeBee Ultralight.mp3");
	SCENE->Object("AUDIO11")->move(12,0,-18);

	(new Cubus("X-7.png"))->SetName("AUDIO12");
	SCENE->Object("AUDIO12")->GetConnected<AudioEmitter>()->LoadeSample("mp3/09-Brite Strings.mp3");
	SCENE->Object("AUDIO12")->move(20,0,-18);


	SCENE->Object("AUDIO01")->GetConnected<AudioEmitter>()->PlayAudio();
	SCENE->Object("AUDIO02")->GetConnected<AudioEmitter>()->PlayAudio();
	SCENE->Object("AUDIO03")->GetConnected<AudioEmitter>()->PlayAudio();
	SCENE->Object("AUDIO04")->GetConnected<AudioEmitter>()->PlayAudio();
	SCENE->Object("AUDIO05")->GetConnected<AudioEmitter>()->PlayAudio();
	SCENE->Object("AUDIO06")->GetConnected<AudioEmitter>()->PlayAudio();
	SCENE->Object("AUDIO07")->GetConnected<AudioEmitter>()->PlayAudio();
	SCENE->Object("AUDIO08")->GetConnected<AudioEmitter>()->PlayAudio();
	SCENE->Object("AUDIO09")->GetConnected<AudioEmitter>()->PlayAudio();
	SCENE->Object("AUDIO10")->GetConnected<AudioEmitter>()->PlayAudio();
	SCENE->Object("AUDIO11")->GetConnected<AudioEmitter>()->PlayAudio();
	SCENE->Object("AUDIO12")->GetConnected<AudioEmitter>()->PlayAudio();

	AUDIO->Play();
	// Music Cube

	(new Cubus("kubismus.png"))->SetName("muckubus");
	SCENE->Object("muckubus")->AddConnectable<Randomover>();
	SCENE->Object("muckubus")->AddConnectable<MusicInteractor>();
	SCENE->Object("muckubus")->GetConnected<MusicInteractor>()->GetLineData(0)->threshold = 1.5;
	SCENE->Object("muckubus")->GetConnected<MusicInteractor>()->GetLineData(1)->threshold = 0.03f;
	SCENE->Object("muckubus")->GetConnected<AudioEmitter>()->LoadeSample("mp3/20-Audio.mp3",false);
	SCENE->Object("muckubus")->GetConnected<AudioEmitter>()->PlayAudio();
	SCENE->Object("muckubus")->GetConnected<AudioEmitter>()->AudioVolume(1);
	SCENE->Object("muckubus")->GetConnected<MusicInteractor>()->automaticFallOffAdjust=false;

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
		SCENE->Object(obj)->AddConnectable<AudioEmitter>();
		SCENE->Object(obj)->GetConnected<AudioEmitter>()->LoadeSample("mp3/03-Kit-808.mp3",false);
		SCENE->Object(obj)->GetConnected<AudioEmitter>()->PlayAudio();
		SCENE->Object(obj)->AddConnectable<MusicInteractor>();
		SCENE->Object(obj)->GetConnected<MusicInteractor>()->automaticFallOffAdjust=false;
		SCENE->Object(obj)->SetName("Brummer");
		SCENE->Object(obj)->IsVisible=true;
		SCENE->Object(obj)->GetConnected<AudioEmitter>()->AudioVolume(1);
	}
	   

	// Spectrum Analyzer
	//analyzer = new SpectrumAnalyzer();
	(new SpectrumAnalyzer())->SetName("SpectrumAnalyzer");
	//analyzer->SetName("SpectrumAnalyzer");
	//analyzer->AddConnectable<CamTargetRotator>();
	SCENE->Object("SpectrumAnalyzer")->AddConnectable<CamTargetRotator>();
	SCENE->Object("SpectrumAnalyzer")->move(0, 0, -30.0f);
	SCENE->Object("SpectrumAnalyzer")->scale(40.0f * 3.5f/128.0f, 0.3f, 2.0f); // 90 ground-tiles * 3.5m width * 128 bands
	((SpectrumAnalyzer*)SCENE->Object("SpectrumAnalyzer"))->Initialize();

	string particleImages[3];
	particleImages[0] = "particle1_128x128.png";
	particleImages[1] = "particle2_128x128.png";
	particleImages[2] = "particle3_128x128.png";

	//(new FogMachine(particleImages))->SetName("DasNebel");
	//SCENE->Object("DasNebel")->move(0,3,0);
	//SCENE->Object("DasNebel")->IsGrounded(false);
	(new ParticleSystem<500>("particle4_128x128.png"));
	SCENE->Object("ParticleSystem")->AddConnectable<ObjectMover<3>>();
	SCENE->Object("ParticleSystem")->GetConnected<ObjectMover<3>>()->AddWaypoint(Vector3(-20,0,40));
	SCENE->Object("ParticleSystem")->GetConnected<ObjectMover<3>>()->AddWaypoint(Vector3(40,0,-20));
	SCENE->Object("ParticleSystem")->GetConnected<ObjectMover<3>>()->AddWaypoint(Vector3(-40,0,20));
	
	SCENE->Object("ParticleSystem")->GetConnected<ObjectMover<3>>()->IsActive = true;
	SCENE->Object("ParticleSystem")->IsGrounded(false);
	SCENE->Object("ParticleSystem")->move(0,0,0);
	SCENE->Object("ParticleSystem")->rotate(3,0.2,-1);
	SCENE->Object("ParticleSystem")->scale(4,4,0);
	SCENE->Object("ParticleSystem")->getTransform()->speed = 20.f;
	((ParticleSystem<500>*)SCENE->Object("ParticleSystem"))->SetColor(255,255,255,25);
	((ParticleSystem<500>*)SCENE->Object("ParticleSystem"))->IsVisible = true;

	// Camera
	SCENE->camera->Mode(FIRSTPERSON);
	SCENE->camera->SetTarget(SCENE->Object("ParticleSystem"));
	//SCENE->camera->ModeSocket->GetCameraMode<TargetGrabber>()->GrabTarget();
	//SCENE->camera->ModeSocket->GetCameraMode<TargetGrabber>()->Mode(TargetGrabber::MODE::ROTATE);

	//overlay = new ScreenOverlay();
	//overlay->Initialize("framen_1920x1080.png");

	AUDIO->BackgroundMusicVolume(0.95);

	AUDIO->Volume(1);
	
	
}



int switcher=0;

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
	
	if(EXIT)
		prepareForExit();
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
	{
		EXIT=true;
	}

	
		

	INPUT->registerKey(key);
}

void keyboardUpInput(unsigned char key,int x,int y)
{
	INPUT->registerKeyUp(key);
}

void processSpecialKeys(int key, int xx, int yy)
{
	//if(key==GLUT_KEY_UP)
	//{
	//	fountain->sensitivity += 1;
	//	printf("fountain-sensitivity: %f\n",fountain->sensitivity);
	//}
	//if(key==GLUT_KEY_DOWN)
	//{
	//	fountain->sensitivity -= 1;
	//	printf("fountain-sensitivity: %f\n",fountain->sensitivity);
	//}

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
