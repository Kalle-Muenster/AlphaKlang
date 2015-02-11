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

void ProjectMappe::LoadContent(void)
{printf("\nENGINE: Loading Scene...\n");

	ProjectMappe::OnLoadContent();

	printf("\nSCENE: Loaded... \n"
		   "...entering game-loop!\n\n");
}

int ProjectMappe::prepareForExit(void)
{printf("\nENGINE: preparing for exit...\n"
		".....     unloading objects...\n");

	glutLeaveGameMode();
	
	//ProjectMappe::GlobalDestructor();
	
	printf("\n     .....unloading done ! \n"
		   "    - EXIT\n");
	return EXIT_SUCCESS;
}






