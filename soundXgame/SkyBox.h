#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#include "VoxGrid.h"
#include "Connectable.h"



class SkyBox :
	public IGObject	
{


public:
	enum WALLS : int
	{NoWall = -1,left=0,right=1,front=2,rear=3,top=4,bottom=5};
	//SkyBox(void);
	SkyBox(string = "FromDaCode");
	virtual void InitializeObject(string ppmFileName, bool addToScene=true);
	virtual ~SkyBox(void);
	VoxGrid* Wall(int);
	virtual void draw(void);
	virtual Vector3 scale(Vector3);
	virtual Vector3 move(Vector3);
	void LoadeHightMap(string filename,int chanel,int wall = -1);
	void LoadeColorMap(string filename,int wall = -1);
	virtual Transform* getTransform(void);

private:
	VoxGrid* walls[6];
	void SetToWall(WALLS);
};


class SkyBoxConnector : public VoxControl
{
public:
	virtual VoxGrid* vConnection(void);
	virtual bool Initialize(void);
	virtual void keyPress(char key);
	
	virtual void DoUpdate(void);

};
#endif