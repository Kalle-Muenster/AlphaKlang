#ifndef __SHADEROBJ_H__
#define __SHADEROBJ_H__

#include "IMeshObject.h"
#include "IGObject.h"
#include "UpdateManager.h"

#include "Shader.h"


class ShaderObj :
	public IMeshObject, public IUpdateble, public Shader
{
private:
public:
	ShaderObj(void);
	~ShaderObj(void);
	virtual void DoUpdate(void);
	
	void InitializeObject(void);
	virtual void draw(void);

};


#endif