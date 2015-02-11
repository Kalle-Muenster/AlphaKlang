#ifndef _CMUSICCONTROLLER_
#define	_CMUSICCONTROLLER_

#include "MusicListener.h"
#include "AudioObject.h"

class MusicControllerPanel;



class MusicController :
	public MusicListener,
	public IConnectable
{

public:
	static bool EditingEnabled;
	MusicController(void);
	virtual ~MusicController(void){}
	virtual void DoUpdate(void);
	bool IsAttachedToPanel;
protected:
	virtual float* GetFFTData(void);
	
};


class MusicControllerPanel 
	: 
	public GuiObject,
	public IUpdateble
{
protected:
	MusicController* controller;

public:
	MusicControllerPanel(void);
	virtual ~MusicControllerPanel(void);
	void BindToMusicListener(MusicController* listener);
	void DetachListener(MusicController* listener);
	MusicController* GetListener(void);
	void UpdateListenerValues(void);
};
#endif