#include "projectGrafics.h"
#include "projectMacros.h"
#include "projectClasses.h"

#include "globalfunctions.h"

//#include "ShaderObj.h"







//Global Declarations:
//int wnd;
//void* font;
 

//Objects:





//void WriteText2D(char*,int,int);


//Entry-point:
int main(int argc,char** argv)
{
	ProjectMappe::InitEngine(argc,argv);
	
	ProjectMappe::LoadContent();

	glutMainLoop();

	return ProjectMappe::prepareForExit();
}



int ProjectMappe::prepareForExit(void)
{
	printf("ENGINE: preparing for exit...\n     .....     unloading objects...\n");
//	glutExit();
	//deletions:
	ProjectMappe::GlobalDestructor();
	
	printf("\n     .....unloading done ! \n    - EXIT\n");
	return EXIT_SUCCESS;
}






//ConID testID;
void ProjectMappe::LoadContent(void)
{
	printf("ENGINE: Loading Scene...\n");
	
	ProjectMappe::OnLoadContent();

	printf("\nSCENE: Loaded... \n ...entering gameloop!\n\n");
}



//int switcher=0;

//Main-Cycle:
////////////////////////////////////////////////////////



//void UpdateCycle(void)
//{
//
//}
//
//
//
//void RenderCycle(void)
//{
//
//}












//GL-InputFunctions:
///////////////////////////////////////////////////




//void WriteText2D(char * text, int x, int y)		
//{
//  char *c;
//  glColor4f(1.0f,1.0f,0.5f,0.2f);
//  glRasterPos2i(x, y);
//  for (c=text; *c != '\0'; c++) 
//  { glutBitmapCharacter(font, *c);}
//}
