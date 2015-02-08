#ifndef _LISTENER_H_
#define _LISTENER_H_

#include "Updatemanager.h"
#include "ControlledValues.h"
#include "Connectable.h"
#include "IEditable.h"


class Listener :
	public IUpdateble, 
	public IConnectable,
	public IEditable
{
protected:
	float*			fftData;
	HCHANNEL		channel;
	virtual void	GetFFTData(void);
	bool			dataRecieved;
	float			MidRange;
public:
	Controlled<float> MotivatorA,MotivatorB,Moderator,follow;
	float splitA,splitB;
	Listener(void);
	virtual ~Listener(void);
	virtual bool Initialize(void);
	virtual void SetListenToChannel(HCHANNEL AudioChannel);
	virtual void DoEarly(void);
	virtual void DoUpdate(void);

};

#endif
