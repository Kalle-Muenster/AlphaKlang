#ifndef _MUSIKUBUS_
#define _MUSIKUBUS_

#include "cubus.h"
#include "IMusicListener.h"

class Musikubus :
	public Cubus,
	public IUpdateble
{
private:
	virtual void DoEarly(void);
	ListenerFunc MoveIt;
	MusicListener* musikInput;
	template<typename DerivedListener> DerivedListener* Create()
	{
		SetID(SCENE->Add(new DerivedListener()));
		return SCENE->Object(LockID());
	}
public:
	Musikubus(void);
	virtual ~Musikubus(void);

	
	

	float* motivators;
	virtual void MoveOnMusic(int line ,ListenerData* data)=0;
};


class MusicBox : public Musikubus
{
private:
	

	
public:
	void Instanciate(void)
	{
	//	Create<>()
	}
};

#endif