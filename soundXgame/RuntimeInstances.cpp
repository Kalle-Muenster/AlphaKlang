/* RuntimeInstances.cpp
 *
 * Instances of all RuntimeManagers are stored and managed here.
 * and most other global variables and objects should move also
 * here too. It's "GlobalDistructor" function should be called to
 * clean up the whole Application-environment when Exiting. 
 * these Runtime-management objects cam become quiet large, so
 * getting their pointers lost at Application-Exit or crash,is not
 * good...
 */


#include "IPrimitive.h"
#include "projectMacros.h"


//Singleton instances..
InputManager*		_inputManagerInstance;
BassAudio*			_bassaudioInstance;
SceneGraph*			_scenegraphInstance;
UpdateManager*		_updateManagerInstance;
GuiManager*			_guimanagerInstance;
ObjectManagement*	_objectManagerInstance;


//Destroy all Singleton-instances...
void 
ProjectMappe::GlobalDestructor(void)
{
	delete _scenegraphInstance;
	delete _guimanagerInstance;
	delete _inputManagerInstance;
	delete _updateManagerInstance;
	delete _objectManagerInstance;
	delete _bassaudioInstance;
};

void
ProjectMappe::StartupRuntimeManagers(void)
{
	printf("Initiating Engine...\n");

	//Invoke all Singletons calling their Constructors at application-start.
	printf(".... %s instantiated !\n",UpdateManager::start());
	printf(".... %s instantiated !\n",InputManager::start());
	printf(".... %s instantiated !\n",BassAudio::start());
	printf(".... %s instantiated !\n",SceneGraph::start());
	printf(".... %s instantiated !\n",GuiManager::start());
	printf(".... %s instantiated !\n",ObjectManagement::start());
	
	

	//Load all Primitive Meshes and generate the Vertex-buffers to GL 
	//and generate static pointers to these buffers. so every 
	//Object will use thees same buffers as their mesh-draw-source... 
	printf(".... loading Primitives !\n");
	InitFlatQuat();
	InitICubic();
	InitIBall();
	InitZylinder();
	InitIspheree();
	InitICone();
	InitICapsule();
	printf("Done ! \n\n");
}


//EXIT:	 - calling this [true] let the application 
//		   begin shutting down the next few frames
//------------------------------------------------
bool _terminate = false;
bool ProjectMappe::EXIT(BOOL exit)
{
	if(exit<3)
		_terminate = exit;
	return _terminate;
}

//Check if engine has entered it's MainLoop-cycle..
bool _isRunning = false;
bool ProjectMappe::isGameRunning(void)
{
	 return _isRunning;
}


// SINGLETON HANDLUING:
////////////////////////////////////////////////
//INPUT: singleton handling
//-------------------------------------------
const char* 
InputManager::start(void)
{
	if(!_inputManagerInstance) 
		_inputManagerInstance = new InputManager();
	return typeid(*_inputManagerInstance).name();
}

InputManager* 
InputManager::getInstance(void) 
{
	return _inputManagerInstance;
}



//GUI - singleton
//-------------------------------------------

const char*
GuiManager::start(void)
{
	if(NotIsInstanciated)
		_guimanagerInstance = new GuiManager();
	return typeid(*_guimanagerInstance).name();
}

GuiManager* 
GuiManager::getInstance(void)
{
	return _guimanagerInstance;
}

//AUDIO - audio interface wrapper (singleton) 
//-------------------------------------------
const char*
BassAudio::start(void)
{
	if(!_bassaudioInstance) 
		_bassaudioInstance = new BassAudio();
	return typeid(*_bassaudioInstance).name();
}

BassAudio*
BassAudio::GetInstance(void)
{
	return _bassaudioInstance;
}


//SCENE - rendering and object-management 
//		  singleton
//-------------------------------------------
const char*
SceneGraph::start(void)
{
	if(!_scenegraphInstance) 
		_scenegraphInstance = new SceneGraph();
	return typeid(*_scenegraphInstance).name();
}

SceneGraph*
SceneGraph::getInstance(void)
{
	return _scenegraphInstance;
}

void
SceneGraph::SetGameIsRunningState(bool isrunning)
{
	_isRunning = isrunning;
}


//UPDATE - singleton-instance for handling all 
//		   per-frame Update callings
//-------------------------------------------
const char*
UpdateManager::start(void)
{
	if(!_updateManagerInstance) 
		_updateManagerInstance = new UpdateManager();
	return typeid(*_updateManagerInstance).name();
}

UpdateManager*
UpdateManager::getInstance(void)
{
	return _updateManagerInstance;
}

//OBJECT - Object-management, generate and supplies Objects
//		   with instance-ID's and component-link-ID's (ConIDs)
//		   and takes care of them being unique.
//--------------------------------------------------------
const char*
ObjectManagement::start(void)
{
	if(NotIsInstanciated)
		_objectManagerInstance = new ObjectManagement();
	return typeid(*_objectManagerInstance).name();
}

ObjectManagement*
ObjectManagement::getInstance(void)
{
	return _objectManagerInstance;
}

