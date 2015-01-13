/* RuntimeInstances.cpp
 *
 * Instances of all RuntimeManagers are stored and managed here.
 * and most other global variables and objects should move also
 * here too. It's "GlobalDistructor" function should be calld to
 * clean up the whole Aplication-environment whenn Exiting. 
 * these Runtimemannagement objects cam become quiet large, so
 * getting their ponters lost at Aplication-Exit or crash,is not
 * good...
 */



#include "projectMacros.h"


InputManager*		_inputManagerInstance;
BassAudio*			_bassaudioInstance;
SceneGraph*			_scenegraphInstance;
UpdateManager*		_updateManagerInstance;
GuiManager*			_guimanagerInstance;
ObjectManagement*	_objectManagerInstance;

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
	printf(".... %s instanciated !\n",UpdateManager::start());
	printf(".... %s instanciated !\n",InputManager::start());
	printf(".... %s instanciated !\n",BassAudio::start());
	printf(".... %s instanciated !\n",SceneGraph::start());
	printf(".... %s instanciated !\n",GuiManager::start());
	printf(".... %s instanciated !\n",ObjectManagement::start());
	printf("Done ! \n\n");
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
SceneGraph::getInstance()
{
	return _scenegraphInstance;
}

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

