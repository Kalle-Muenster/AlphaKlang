#ifndef _Kubus_h_
#define _Kubus_h_

#include "SceneGraph.h"
#include "connectable.h"
#include "IMeshObject.h"



class Kubus :
	public IObjection<ICubeXtor>
{
protected:
	virtual bool useTexture(BOOL=3);

public:
	Kubus(void);
	virtual ~Kubus(void);
	virtual void InitializeObject(void);
	virtual void draw(void);
	virtual operator ICubeXtor(void);
	virtual void LoadTexture(string textureFileName);
};


#endif
