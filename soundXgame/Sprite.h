#ifndef _SPRITE_H_
#define _SPRITE_H_



#include "IMeshObject.h"



class Sprite :
	public IMeshObject
{

public:
	virtual Texture* GetTexture(void);
	virtual void	 SetTexture(Texture);
	Sprite(char*,bool addToScene = true);
	Sprite(Texture,bool addToScene = true);
	Sprite(void);
	virtual ~Sprite(void);
	void SetSomeValue(float);

	void SetUp(string textureFile,bool backface=true,bool addToScene=true);
	void SetUp(Texture texture,bool backface=true);
};



#endif