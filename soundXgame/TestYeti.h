#ifndef _TESTYETI_H_
#define _TESTYETI_H_

#include "Observer.h"
#include "IGObject.h"
#include "IMeshObject.h"
#include "Connectable.h"
#include "IMusicListener.h"

class TestYeti :
	public IMeshObject ,  
	public IUpdateble
{
	public:

	unsigned int ConIDs[MAXIMUM_NUMBER_OF_CONNECTIONS]; 
	int* NumberOfConnectables;
	ListenerFunc MoveIt;

	virtual void KeyPress(char key)
	{
			printf("%c-Pressed! - recieved by TestYeti",key);
	}

	 void MoveOnMusic(int line ,ListenerData* data);
	
	

	virtual void DoUpdate(void);


	TestYeti(const char* objFile,const char* textureFile,bool addToSceneGraph);


	virtual ~TestYeti(void);
};

#endif