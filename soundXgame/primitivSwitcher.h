#ifndef _PIMITIV_SWITCHER_H_
#define _PIMITIV_SWITCHER_H_

#include "Connectable.h"
#include "UpdateManager.h"

class primitivSwitcher :
	public IConnectable,
	public IUpdateble
{
protected:
	bool switched;
public:
	enum PRIMITIVES : short
	{CUBE,BALL,SPHERE,ZYLINDER,CONE,CAPSULE,FLATQUAD};
	short currentSellected;
	primitivSwitcher(void);
	virtual ~primitivSwitcher(void);
	virtual bool Initialize(void);
	virtual void DoUpdate(void);
	void Switch(PRIMITIVES);
};

#endif