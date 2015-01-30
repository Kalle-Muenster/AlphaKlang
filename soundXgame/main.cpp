#include "projectGrafics.h"
#include "projectMacros.h"
#include "projectClasses.h"

#include "globalfunctions.h"


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




