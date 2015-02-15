#include "projectGrafics.h"
#include "projectMacros.h"
#include "globalfunctions.h"


unsigned int _wnd;

void 
ProjectMappe::InitEngine(int &argc,char** argv)
{
	glutInit(&argc,argv);

	InitGlut();
	GlInit();

	ProjectMappe::StartupRuntimeManagers();

	INPUT->SetDoubleclickTime(400);


	AUDIO->Set3D_DopplerFXFactor(0.66f);

	AUDIO->Set3D_DistanceFactor(1.0f);
	AUDIO->Set3D_RollOffFactor(10.0f);
	//AUDIO->Volume(0);
}

void 
ProjectMappe::InitGlut(void)
{

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowSize(SCREENWIDTH,SCREENHEIGHT);
	_wnd = glutCreateWindow("AlphaKlang by Kalle Münster and Dario D. Müller [www.game-coding.com]");
//	ProjectMappe::font = GLUT_BITMAP_HELVETICA_18;

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
	//	 

	GLenum glewError = glewInit();
	if( glewError != GLEW_OK )
	{
		std::cerr << "Unable to init GLew";
	}

}

void 
ProjectMappe::GlInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClearDepth(1);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	// glDisable( GL_LIGHTING );
	glEnable( GL_DITHER );


	// Disable dithering



	glEnable(GL_MULTISAMPLE_ARB); // enable Multi-sample Anti-aliasing
	glEnable(GL_BLEND); // enable transparency

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // do transparency
}

unsigned int
ProjectMappe::GetGLWindowID(void)
{
	return _wnd;
}