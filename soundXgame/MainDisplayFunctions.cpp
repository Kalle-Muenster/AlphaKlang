#include "projectGrafics.h"
#include "projectMacros.h"
#include "Camera.h"

#include "globalfunctions.h"

//#define LATE_AFTER_DRAW
#define LATE_BEFOR_DRAW

//GL-DisplayCallbacks
////////////////////////////////////////////////////////

void ProjectMappe::OnIdle(void)
{
	glutPostRedisplay();
}

void ProjectMappe::OnReshape(GLsizei size_x,GLsizei size_y)
{
	if(size_y == 0)
		size_y = 1;

	INPUT->SaveViewportRectangle(0,0,size_x,size_y);
	SCENE->camera->Aspect((float)size_x/(float)size_y);
}

void ProjectMappe::OnDisplay(void)
{
	if(SCENE->camera->NeedViewUpdate)
	{
		glutReshapeWindow(SCREENWIDTH+1,SCREENHEIGHT+1);
		SCENE->camera->NeedViewUpdate=false;
	}

	UpdateCycle();

#ifdef LATE_BEFOR_DRAW
	UPDATE->DoTheLateUpdates();
#endif


	SCENE->DrawSky();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RenderCycle();
	glutSwapBuffers();

#ifdef LATE_AFTER_DRAW
	UPDATE->DoTheLateUpdates();
#endif

	INPUT->PerFrameReset();
	AUDIO->PerFrameReset();

	if(EXIT())
		prepareForExit();
}
