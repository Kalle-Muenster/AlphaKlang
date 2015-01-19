#ifndef _LISTENER_H_
#define _LISTENER_H_

#include "Updatemanager.h"
#include "ControlledValues.h"
#include "Connectable.h"

class Listener :
	public IUpdateble, public IConnectable
{
protected:
	float*			fftData;
	HCHANNEL		channel;
	virtual void	GetFFTData(void);

public:
	Controlled<float> MotivatorA,MotivatorB,TriggerA,TriggerB;
	Listener(void);
	virtual ~Listener(void);
	virtual bool Initialize(void);
	virtual void ListenToChannel(HCHANNEL AudioChannel);
	virtual void DoEarly(void);
	virtual void DoUpdate(void);

};

#endif
