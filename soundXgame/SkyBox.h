#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#include "VoxGrid.h"
#include "Connectable.h"



class SkyBox :
	public IGObject	
{


public:
	enum WALLS : int
	{left,right,front,rear,top,bottom};
	//SkyBox(void);
	SkyBox(string = "FromDaCode");
	virtual void InitializeObject(string ppmFileName, bool addToScene=true);
	virtual ~SkyBox(void);
	VoxGrid* Wall(int);
	virtual void draw(void);
	virtual Vector3 scale(Vector3);
	virtual Vector3 move(Vector3);
	void LoadeHightMap(string filename,int chanel,int wall=-1);
	void LoadeColorMap(string filename,int wall=-1);
	

private:
	VoxGrid* walls[6];
	void SetWallvConnection(WALLS);
};


class SkyBoxConnector : public VoxControl
{
public:
	virtual IGObject* Connection(void);
	virtual VoxGrid* vConnection(void);
	virtual bool Initialize(void);
	virtual void keyPress(char key);

	virtual void DoUpdate(void);

};
#endif