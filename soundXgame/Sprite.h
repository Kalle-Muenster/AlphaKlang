#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "IMeshObject.h"


class Sprite :
	public IMeshObject
{
private:
	void SetUp(string textureFile,bool backface=true); 
	void SetUp(Texture texture,bool backface=true);
protected:
	Texture* pTexture;
public:
	virtual Texture* GetTexture(void);
	virtual void	 SetTexture(Texture*);
	Sprite(void);
	Sprite(char*,bool addToScene = true);
	virtual ~Sprite(void);
};

#endif