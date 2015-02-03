#ifndef __ISCHMENA_H__
#define __ISCHMENA_H__


#include <vector>
#include "projectGrafics.h"
#include "Connectable.h"



class IGObject 
	: public IObjection<IConXtor>
{
protected:
	bool		SetID(GobID);
	unsigned	LockID(void);
	void*		pUserData;

public:
	IGObject(void);
	virtual ~IGObject(void);
	void			InitializeObject(bool addToSceneGraph=true);
	virtual Transform*		getTransform(void);
	virtual GobID	GetID(void);
	virtual const char*	GetName(void);
	virtual void	SetName(char*);
	virtual operator IConnectable();
	float			SomeValue;
	virtual void	Action(IConnectable* sender);

	template<typename T> bool HasConnected(void)
	{
		int i = -1;
		size_t t = typeid(T).hash_code();
		while((conXtor->ConIDs[++i]!=t)	&& (i<IConnectable::MaximumNumberOfConnectioms));
		if(i<IConnectable::MaximumNumberOfConnectioms)
			return true;
		else
			return false;
	}
};


#endif