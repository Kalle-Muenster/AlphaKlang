#ifndef _FOGMACHINE_H_
#define _FOGMACHINE_H_

#define MAXIMUM_NUMBER_OF_PARTICLES	(1000)

#include "LueckList.h"
#include "Sprite.h"
#include "UpdateManager.h"

class FogMachine :
	public IGObject,
	public IUpdateble
	{
	protected:
		LueckList<Sprite*,MAXIMUM_NUMBER_OF_PARTICLES> fogs;
		Texture texture;
		bool particlesAlwaysFaceCamera;
	public:
		FogMachine(void);
	//	virtual void InitializeObject(void);
		virtual ~FogMachine(void);

		float varianz;
		float accselleration;
		float emittingAngle;

		void Loade(char* nebelFile);
		void MachDampf(void);
		virtual void draw(void);
		virtual void DoUpdate(void);
	};

#endif