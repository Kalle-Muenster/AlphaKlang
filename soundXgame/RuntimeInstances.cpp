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



#include "projectMacros.h"


InputManager*		_inputManagerInstance;
BassAudio*			_bassaudioInstance;
SceneGraph*			_scenegraphInstance;
UpdateManager*		_updateManagerInstance;
GuiManager*			_guimanagerInstance;
ObjectManagement*	_objectManagerInstance;

//A data-array which can be used as frame buffer-memory for applying image-effects to the screen.. 
//data32 screenBuffer[1920*1080];

void 
ProjectMappe::GlobalDestructor(void)
{
	delete _inputManagerInstance;
	delete _bassaudioInstance;
	delete _scenegraphInstance;
	delete _updateManagerInstance;
	delete _guimanagerInstance;
	delete _objectManagerInstance;
};

void
ProjectMappe::StartupRuntimeManagers(void)
{
	printf("Initiating Engine...\n");
	printf(".... %s instantiated !\n",UpdateManager::start());
	printf(".... %s instantiated !\n",InputManager::start());
	printf(".... %s instantiated !\n",BassAudio::start());
	printf(".... %s instantiated !\n",SceneGraph::start());
	printf(".... %s instantiated !\n",GuiManager::start());
	printf(".... %s instantiated !\n",ObjectManagement::start());
	printf("Done ! \n\n");
}

bool _terminate=false;
bool ProjectMappe::EXIT(BOOL exit)
{
	if(exit<3)
		_terminate = exit;
	return _terminate;
}

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
/*
data32* 
SceneGraph::GetFXbufferData(void)
{
	return &screenBuffer[0];
}
 */
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

