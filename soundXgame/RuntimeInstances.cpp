/* RuntimeInstances.h
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


InputManager*	 _inputManagerInstance;
BassAudio*		 _bassaudioInstance;
SceneGraph*		 _scenegraphInstance;
UpdateManager*	 _updateManagerInstance;

static void GlobalDestructor(void)
{
	delete _inputManagerInstance;
	delete _bassaudioInstance;
	delete _scenegraphInstance;
	delete _updateManagerInstance;
};

//-------------------------------------------

InputManager* 
InputManager::getInstance(void) 
{
	if(!_inputManagerInstance) 
	{
		_inputManagerInstance = new InputManager();
	}

	return _inputManagerInstance;
}

InputManager::~InputManager(void)
{
	delete _inputManagerInstance;
}

//-------------------------------------------

BassAudio*
BassAudio::GetInstance(void)
{
	if(!_bassaudioInstance)
		_bassaudioInstance = new BassAudio();
	return
		_bassaudioInstance;
}

BassAudio::~BassAudio(void)
{
	delete _bassaudioInstance;
}

//-------------------------------------------

SceneGraph*
SceneGraph::GetInstance()
{
	if(!_scenegraphInstance)
		_scenegraphInstance = new SceneGraph();
	return _scenegraphInstance;
}

//-------------------------------------------

UpdateManager*
UpdateManager::getInstance(void)
{
	if(!_updateManagerInstance)
		_updateManagerInstance = new UpdateManager();
	return _updateManagerInstance;
}