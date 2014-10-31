#include "projectGrafics.h"
#include "projectMacros.h"
#include "projectClasses.h"

//Global Declerations:
////////////////////////////////////////////
//vars:
int wnd;
void* font;

//Objects:
Map* map;
TestYeti* testYeti;

//Functions:
void Init(void);
void LoadContent(void);
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
// unsigned create_shader(const char* shaderCodeFileName,GLenum type);
////////////////////////////////////////////


//int atribut_coordinate2;;


//Entrypoint:
int main(int argc,char** argv)
{
	glutInit(&argc,argv);
	Init();

	glutMainLoop();
	
	return prepareForExit();
}

int prepareForExit(void)
{
	//deletions:
	delete font;
	delete map;
	delete testYeti;

	return 0;
}

void Init(void)
{
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA);
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
	glewInit();

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1);

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//shader
	 
//	glEnableVertexAttribArray( atribut_coordinate2 );

	LoadContent();
}

void LoadContent(void)
{
	INPUT->attachMouseWheel(SCENE->camera);

//	AUDIO->LoadeBackgroundAudio("testtrack.mp3");
//	AUDIO->Play();

	testYeti = new TestYeti("wendy_Scene.obi","tex_wendy.jpg",true);
	map = new Map("Landschaft.obi","Landschaft_Diffuse.jpg",true);
	map->move(Vector3(map->getTransform()->position.x,-0.2,map->getTransform()->position.z));

	SCENE->camera->SetTarget(testYeti);
	testYeti->move(testYeti->getTransform()->position.x-3,testYeti->getTransform()->position.y,testYeti->getTransform()->position.z);
}

//Main-Cycle:
////////////////////////////////////////////////////////
void UpdateCycle(void)
{
	UPDATE->DoAllTheUpdates();
}

void RenderCycle(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

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


//GL-InputFunctions;
///////////////////////////////////////////////////
//Keyboard:
void keyboardInput(unsigned char key,int x,int y)
{
	if(key=='q')
		glutExit();

	if(key=='o')
		SCENE->camera->Mode(ORTHOGRAFIC);

	if(key=='p')
		SCENE->camera->Mode(PERSPECTIVE);

	INPUT->notifyKey(key);
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



//unsigned create_shader(const char* shaderCodeFileName,GLenum type)
//{
//	unsigned shaderV =	glCreateShader(type);
//	FILE* shaderfile = fopen(shaderCodeFileName,"rb");
//	fseek(shaderfile,0,SEEK_END);
//	int size = ftell(shaderfile);
//	fseek(shaderfile,0,SEEK_SET);
//	unsigned char* shaderCodeString;
//
//
//}