#include "projectGrafics.h"
#include "projectMacros.h"
#include "projectClasses.h"

#include "ShaderObj.h"

//Global Declerations:
////////////////////////////////////////////
//vars:
int wnd;
void* font;

//Objects:
SpectrumAnalyzer* analyzer;

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
void MouseHoverWindow(int);
void GamePadFunc(unsigned,int,int,int);
int prepareForExit(void);

////////////////////////////////////////////
//Entrypoint:
int main(int argc,char** argv)
{
	glutInit(&argc,argv);

	InitGlut();
	GlInit();
	LoadContent();

	//glewInit(); <-- already in InitGlut() Method

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
	glutInitWindowSize(SCREENWIDTH, SCREENHEIGHT);
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
	glutKeyboardFunc(keyboardInput);
	glutEntryFunc(MouseHoverWindow);

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
	
	// enable Multisample Antialiasing
    glEnable(GL_MULTISAMPLE_ARB);
	
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//shader
	//glEnableVertexAttribArray( atribut_coordinate2 );
}


//ConID* testID;
//int i1,i2,i3;
//int cycle1 = 127;
//int cycle2 = 255;
//int cycle3 = 127;
void LoadContent(void)
{
	//i1 = -1;
	//i2 = -2;
	//i3 = 1;

	AUDIO->LoadeBackgroundAudio("testtrack.mp3");
	AUDIO->Play();

	// Gameplay Objects
	Ground* ground = Ground::getInstance();
	Fountain* fountain = new Fountain();
	ShaderObj* shaderObj = new ShaderObj();


	// TEST ... JUST A CUBE - 1
	new Cubus("X-3.png", true, true);
	


	// TEST ... JUST A CUBE - 2
	data32 color;
	color.byte[1]=0;
	color.byte[2]=255;
	color.byte[3]=127;
	color.byte[0]=10;
	(new Cubus(color,true,false,Vector3(0,0,0)))->scale(Vector3(10,10,10));



	data32 col = data32();
	col.byte[0] = 255;
	col.byte[1] = 200;
	col.byte[2] = 150;
	col.byte[3] = 100;
	int i = -1;



	//Voxelplane... 
	//on runtime use press"X"to choose a an Image file from list (watch console!)
	//press "R" to loade the sellected image as Image.
	//press "L" to loade the sellected image'as Bumpmap. -> press several times each colorchannl seperate...
	VoxGrid* vObject = new VoxGrid("drei_.ppm");
	vObject->AddConnectable<VoxControl>(); // <-- look keyInput-function for more detail...
	vObject->GetConnected<VoxControl>()->Connection()->SetName("voxels");
	vObject->move(-80,0,150);
	vObject->MainSizzes.x=0.2;;
	vObject->MainSizzes.y=0.045f;




	SCENE->camera->Mode(FIRSTPERSON);

	analyzer = new SpectrumAnalyzer();
	analyzer->SetName("SpectrumAnalyzer");
	analyzer->AddConnectable<CamTargetRotator>();
	SCENE->Object(analyzer->GetID())->move(1,1,-2);
	SCENE->Object("SpectrumAnalyzer")->move(1,10,-2);
	analyzer->IsGrounded = true;
	SCENE->camera->SetTarget(vObject);

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
	UPDATE->DoAllTheUpdates();

	if(INPUT->Mouse.RIGHT.CLICK)
	{
		if(++switcher>=SCENE->ObjectsCount())
			switcher=1;
		
		SCENE->camera->SetTarget(SCENE->Object(switcher));
	}

	if(INPUT->Mouse.WheelV==WHEEL::UP)
	{
		analyzer->fallOffAmount += 0.01f;
		printf("FallOffAmount: %f",analyzer->fallOffAmount);
	}

	if(INPUT->Mouse.WheelV==WHEEL::DOWN)
	{
		analyzer->fallOffAmount -= 0.01f;
		printf("FallOffAmount: %f",analyzer->fallOffAmount);
	}
}

//The Main-Draw-Call...
void RenderCycle(void)
{
	SCENE->DrawSky();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SCENE->DrawAll();
		
	glutSwapBuffers();
}


//GL-DisplayCallbacks
////////////////////////////////////////////////////////

void OnDisplay(void)
{
	UpdateCycle();
	RenderCycle();
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


char lastKey = 0;
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


	INPUT->notifyKey(key);

	lastKey=key;
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

void MouseHoverWindow(int)
{

}
