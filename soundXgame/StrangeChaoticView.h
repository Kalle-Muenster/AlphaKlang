#ifndef _STRANGECHAOTICVIEWMODE_
#define _STRANGECHAOTICVIEWMODE_

#include "Cam.h"

class StrangeChaoticView :
	public CameraMode
{
public:
	class ViewConvergencer :
	public IGObject
	{
	protected:
		Texture texture;
		List<glm::vec3*,4> verts;
		List<glm::vec2*,4> uvs;
		data32 color;
	
	public:
		ViewConvergencer(void);
		virtual void Initialize(void);
		virtual ~ViewConvergencer(void);
		virtual void draw(void);
		void LoadTexture(char* = "ImageFile");
		ProjectMappe::Rectangle Position(float,float,float,float);
		ProjectMappe::Rectangle Position(ProjectMappe::Rectangle);
		bool UseTexture;
		bool IsActive;
	};

protected:
	virtual void UpdateMode(void);
	ViewConvergencer* convergencer;
public:
	static int ID;
	StrangeChaoticView(void);
	virtual ~StrangeChaoticView(void);
	virtual bool Initialize(void);
	virtual bool Activate(BOOL = 3);
};

#endif
