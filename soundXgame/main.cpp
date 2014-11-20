#include "projectGrafics.h"
#include "projectMacros.h"
#include "projectClasses.h"


//Global Declerations:
////////////////////////////////////////////
//vars:
int wnd;
void* font;

//Objects:
Ground* ground;
Fountain* fountain;


//Functions:
void Init(void);
void GlInit(void);
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

// Shader
int initResourcesShader();
void OnDisplayShader();
void freeResourceShader();



////////////////////////////////////////////
//Entrypoint:
int main(int argc,char** argv)
{
	glutInit(&argc,argv);
	Init();

	
	GLenum glew_status = glewInit();

	glutMainLoop();
	
	return prepareForExit();
}

int prepareForExit(void)
{
	//deletions:
	delete font;

	// Shader
	freeResourceShader();

	return EXIT_SUCCESS;
}

void Init(void)
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
	
	// Shader
	initResourcesShader();

	GlInit();

	LoadContent();
}

void GlInit(void)
{
	
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
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
ConID* testID;
int i1,i2,i3;
int cycle1 = 127;
int cycle2 = 255;
int cycle3 = 127;
void LoadContent(void)
{
i1 = -1;
i2 = -2;
i3 = 1;
	AUDIO->LoadeBackgroundAudio("testtrack.mp3");
	AUDIO->Play();

	ground = Ground::getInstance();
	ground->IsVisible = true;

	fountain = new Fountain();
	

	Cubus* musikubus;
	data32 col = data32();
	col.byte[0] = 255;
	col.byte[1] = 200;
	col.byte[2] = 150;
	col.byte[3] = 100;
	int i = -1;
	IGObject* tempObject = new Cubus(col);
	(new Cubus())->LoadTexture("X-512.jpg")->move(i,tempObject->getTransform()->position.y,i++);
	((Cubus*)SCENE->Object((unsigned)2))->UseTexture =true;;
	(new MrZylinder())->LoadTexture("Deckelblech128-1.png")->move(i,tempObject->getTransform()->position.y,i++);
	(new Sphere())->LoadTexture("Deckelblech-2.png")->move(i,tempObject->getTransform()->position.y,i++);
	(new Cubus())->LoadTexture("Deckelblech128-2.png")->move(i,tempObject->getTransform()->position.y,i++);
	(musikubus=new Cubus("X-7.tga",false))->move(i,tempObject->getTransform()->position.y,i++);
	
	VoxGrid* vObject = new VoxGrid("drei_.ppm");
	vObject->AddConnectable<VoxControl>();
	vObject->GetConnected<VoxControl>()->Connection()->SetName("voxels");
	vObject->AddConnectable<CamTargetRotator>();
	(new Sprite())->move(0,2,0);
	(new Sprite())->LoadTexture("Deckelblech128.tga")->move(2,2,0);

//	IGObject* tempObject = SCENE->camera->SetTarget((new Cubus("X-7.png",true)));


	
	
	SCENE->camera->SetTarget(tempObject);
	
	
	(new Sprite())->move(0,2,0);
	(new Sprite())->LoadTexture("Deckelblech128.tga")->move(2,2,0);


	SCENE->camera->Mode(FIRSTPERSON);

	((Cubus*)SCENE->Object((unsigned)1))->UseTexture = false;
}



GobID switcher=0;
//Main-Cycle:
////////////////////////////////////////////////////////
void UpdateCycle(void)
{
	//color test flashing krams....
	if(cycle1<0||cycle1>255)
		i1= -i1;
	if(cycle2<0||cycle2>255)
		i2= -i2;
	if(cycle3<0||cycle3>255)
		i3= -i3;

	cycle1+=i1;
	cycle2+=i2;
	cycle3+=i3;

	((Cubus*)SCENE->Object((unsigned)1))->color.byte[1] = cycle1;
	((Cubus*)SCENE->Object((unsigned)1))->color.byte[2] = cycle2;
	((Cubus*)SCENE->Object((unsigned)1))->color.byte[3] = cycle3;
	((Cubus*)SCENE->Object((unsigned)1))->color.byte[0] = (255-cycle1);

	data32 col = ((Cubus*)SCENE->Object((unsigned)0))->color;
	//printf("color: %i,%i,%i,%i\n",col.byte[1] ,col.byte[2] ,col.byte[3] ,col.byte[0] );
	//___________________________________________________________



	//Update:
	UPDATE->DoAllTheUpdates();

	if(INPUT->Mouse.RIGHT.CLICK)
	{
		SCENE->camera->SetTarget(SCENE->Object(switcher));
		if(++switcher>=SCENE->ObjectsCount())
			switcher=0;
	}
	
}

void RenderCycle(void)
{
	SCENE->DrawSky();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SCENE->DrawAll();
OnDisplayShader();
		
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
	/* Switches Cam-Modes..*/
	if(key=='o')
	{	SCENE->camera->ModeSocket->GetCameraMode<FirstPerson>(FirstPerson::StaticCamModeID)->IsActive=true;
		SCENE->camera->ModeSocket->GetCameraMode<Spectator>(Spectator::StaticCamModeID)->IsActive=false;}
	if(key=='p')
	{	SCENE->camera->ModeSocket->GetCameraMode<Spectator>(Spectator::StaticCamModeID)->IsActive=true;
		SCENE->camera->ModeSocket->GetCameraMode<FirstPerson>(FirstPerson::StaticCamModeID)->IsActive=false;}

	if(key == 27) // ESC
		glutExit();


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



/* * * * * * * * * * S * H * A * D * E * R * * * * * * * * * * * */
/*
#define _USE_MATH_DEFINES

#include <stdio.h>
#include <stdlib.h>

#include <GL/glew.h>
// GLUT für Fensterintialisierung
#include <GL/freeglut.h>
*/

#include <math.h>
#include "Shader.h"

GLuint program;

// Attribute + Uniforms für Vertices und Farbe
GLint attribute_coord2d, attribute_v_color;
GLint uniform_alpha;

// Buffer für Vertices und Farben
GLuint vbo_triangle, vbo_triangle_colors;

int initResourcesShader()
{
	GLint compile_ok = GL_FALSE;
	GLint link_ok = GL_FALSE;

	GLuint vs, fs;

	if ((vs = Shader::createShader("data/Shader/triangle.v.glsl", GL_VERTEX_SHADER)) == 0)
		return 0;
	if ((fs = Shader::createShader("data/Shader/triangle.f.glsl", GL_FRAGMENT_SHADER)) == 0)
		return 0;

	// program = Kombination aus Fragment und Vertex Shader
	// arbeiten zusammen und vertex shader kann zusätzliche daten an fragment shader übermitteln
	program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);

	// Parameter eines Programobjekts zurückgeben
	glGetProgramiv(program, GL_LINK_STATUS, &link_ok);

	if (!link_ok)
	{
		//std::cerr << "glLinkProgram:";
		fprintf(stderr, "glLinkProgram:");
		return 0;
	}

	//vermischen von vertex und farbwerten
	
	const char* attribute_name = "coord2d";

	// Speicherort eines Attributs zurüchgeben
	attribute_coord2d = glGetAttribLocation(program, attribute_name);

	if (attribute_coord2d == -1)
	{
		//std::cerr << "Could not bind attribute %s\n" << attribute_name;
		fprintf(stderr, "Could not bind attribute %s\n", attribute_name);
		return 0;
	}

	attribute_name = "v_color";
	attribute_v_color = glGetAttribLocation(program, attribute_name);
	if (attribute_v_color == -1)
	{
		fprintf(stderr, "Could not find attribute %s\n", attribute_name);
		return 0;
	}
	
	GLfloat triangle_attributes[] = {
		 0.0,  0.8,		1.0, 0.0, 0.0,
		-0.8, -0.8,		0.0, 1.0, 0.0,
		 0.8, -0.8,		0.0, 0.0, 1.0,
	};

	glGenBuffers(1, &vbo_triangle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_attributes), triangle_attributes, GL_STATIC_DRAW);

	return 1;
}

void OnDisplayShader()
{
	
	// Program verwenden
	glUseProgram(program);

	//vertexattributepointer farbe + vertex
	glEnableVertexAttribArray(attribute_coord2d);
	glEnableVertexAttribArray(attribute_v_color);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_triangle);

	glVertexAttribPointer(
		attribute_coord2d,   // attribute
		2,                   // anzahl der elemente pro vertex
		GL_FLOAT,            // datentyp der elemente
		GL_FALSE,            // werte verwenden wie sie sind
		5 * sizeof(GLfloat), // nächste coord2d erscheint alle 5 elemente
		0                    // offset vom ersten element
		);
	glVertexAttribPointer(
		attribute_v_color,					// attribute
		3,									// anzahl der elemente pro vertex
		GL_FLOAT,							// datentyp der elemente
		GL_FALSE,							// werte verwenden wie sie sind
		5 * sizeof(GLfloat),				// nächste coord2d erscheint alle 5 elemente
		(GLvoid*)(2 * sizeof(GLfloat))		// offset vom ersten element
		);

	// Grafikprimitive mit Arraydaten zeichnen
	// Params: mode, startindex, anzahl der indizes die gerendert werden sollen
	glDrawArrays(GL_TRIANGLES, 0, 3);

	// aktivieren eines vertex attribute arrays
	glDisableVertexAttribArray(attribute_coord2d);
	glDisableVertexAttribArray(attribute_v_color);

	glUseProgram(0);

}

void freeResourceShader()
{
	glDeleteProgram(program);
	glDeleteBuffers(1, &vbo_triangle);
	glDeleteBuffers(1, &vbo_triangle_colors);
}


