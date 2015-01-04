#ifndef __VOXCONTROLL_H__
#define __VOXCONTROLL_H__

#include "InputManager.h"
#include "Connectable.h"
//#include "VoxGrid.h"

class VoxGrid;

class VoxControl :
	public IConnectable, public IInteractive, public IUpdateble
{
protected:
	Vector3 tempvector;
	int bumpmapchannel;
	int imageIndex;
	int bumpmapIndex;
	bool transparenseEnabled;
	float timer;
	bool ZedMode;
	float YPS;
	static string files[6];
	char lastKey;
	int currentSellection;
	float bumper;

public:
	VoxControl(void);
	virtual ~VoxControl(void);
	virtual bool Initialize(void);
	virtual void keyPress(char key);
	//virtual IGObject* Connection(void);
	virtual VoxGrid* vConnection(void);
	virtual void DoUpdate(void);

	//template<typename V> V* Get(ConID id = NULL)
	//{
	//	if(id>=0)
	//		(V*)getConnectables(id);
	//	else
	//	{
	//		size_t t = typeid(V).hash_code();
	//		if(typeid(this) != t)
	//		{
	//			for(int i=0;i<IConnectable::MaximumNumberOfConnections;i++)
	//			{
	//				if(ConIDs[i]==t)
	//					return (V*)getConnectables(i);
	//			}
	//		}
	//		else return this;
	//	}
	//	return false;
	//}

};



#endif